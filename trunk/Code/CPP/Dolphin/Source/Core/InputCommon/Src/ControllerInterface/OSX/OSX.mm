#include <Foundation/Foundation.h>
#include <IOKit/hid/IOHIDLib.h>

#include "../ControllerInterface.h"
#include "OSX.h"
#include "OSXKeyboard.h"
#include "OSXJoystick.h"

namespace ciface
{
namespace OSX
{


static IOHIDManagerRef HIDManager = NULL;
static CFStringRef OurRunLoop = CFSTR("DolphinOSXInput");
static std::map<std::string, int> kbd_name_counts, joy_name_counts;

void DeviceElementDebugPrint(const void *value, void *context)
{
	IOHIDElementRef e = (IOHIDElementRef)value;
	bool recurse = false;
	if (context)
		recurse = *(bool*)context;

	std::string type = "";
	switch (IOHIDElementGetType(e)) {
	case kIOHIDElementTypeInput_Axis:
		type = "axis";
		break;
	case kIOHIDElementTypeInput_Button:
		type = "button";
		break;
	case kIOHIDElementTypeInput_Misc:
		type = "misc";
		break;
	case kIOHIDElementTypeInput_ScanCodes:
		type = "scancodes";
		break;
	case kIOHIDElementTypeOutput:
		type = "output";
		break;
	case kIOHIDElementTypeFeature:
		type = "feature";
		break;
	case kIOHIDElementTypeCollection:
		type = "collection";
		break;
	}

	std::string c_type = "";
	if (type == "collection")
	{
		switch (IOHIDElementGetCollectionType(e)) {
		case kIOHIDElementCollectionTypePhysical:
			c_type = "physical";
			break;
		case kIOHIDElementCollectionTypeApplication:
			c_type = "application";
			break;
		case kIOHIDElementCollectionTypeLogical:
			c_type = "logical";
			break;
		case kIOHIDElementCollectionTypeReport:
			c_type = "report";
			break;
		case kIOHIDElementCollectionTypeNamedArray:
			c_type = "namedArray";
			break;
		case kIOHIDElementCollectionTypeUsageSwitch:
			c_type = "usageSwitch";
			break;
		case kIOHIDElementCollectionTypeUsageModifier:
			c_type = "usageModifier";
			break;
		}
	}

	c_type.append(" ");
	NSLog(@"%s%s%spage: 0x%x usage: 0x%x name: %@ "
		"lmin: %ld lmax: %ld pmin: %ld pmax: %ld",
		  type.c_str(),
		  type == "collection" ? ":" : "",
		  type == "collection" ? c_type.c_str() : " ",
		  IOHIDElementGetUsagePage(e),
		  IOHIDElementGetUsage(e),
		  IOHIDElementGetName(e),		// usually just NULL
		  IOHIDElementGetLogicalMin(e),
		  IOHIDElementGetLogicalMax(e),
		  IOHIDElementGetPhysicalMin(e),
		  IOHIDElementGetPhysicalMax(e));

	if ((type == "collection") && recurse)
	{
		CFArrayRef elements = IOHIDElementGetChildren(e);
		CFRange range = {0, CFArrayGetCount(elements)};
		// this leaks...but it's just debug code, right? :D
		CFArrayApplyFunction(elements, range,
			DeviceElementDebugPrint, NULL);
	}
}

void DeviceDebugPrint(IOHIDDeviceRef device)
{
#if 0
#define shortlog(x) NSLog(@"%s: %@", \
		x, IOHIDDeviceGetProperty(device, CFSTR(x)));
	NSLog(@"-------------------------");
	NSLog(@"Got Device: %@",
		IOHIDDeviceGetProperty(device, CFSTR(kIOHIDProductKey)));
	shortlog(kIOHIDTransportKey)
	shortlog(kIOHIDVendorIDKey)
	shortlog(kIOHIDVendorIDSourceKey)
	shortlog(kIOHIDProductIDKey)
	shortlog(kIOHIDVersionNumberKey)
	shortlog(kIOHIDManufacturerKey)
	shortlog(kIOHIDProductKey)
	shortlog(kIOHIDSerialNumberKey)
	shortlog(kIOHIDCountryCodeKey)
	shortlog(kIOHIDLocationIDKey)
	shortlog(kIOHIDDeviceUsageKey)
	shortlog(kIOHIDDeviceUsagePageKey)
	shortlog(kIOHIDDeviceUsagePairsKey)
	shortlog(kIOHIDPrimaryUsageKey)
	shortlog(kIOHIDPrimaryUsagePageKey)
	shortlog(kIOHIDMaxInputReportSizeKey)
	shortlog(kIOHIDMaxOutputReportSizeKey)
	shortlog(kIOHIDMaxFeatureReportSizeKey)
	shortlog(kIOHIDReportIntervalKey)
	shortlog(kIOHIDReportDescriptorKey)
#endif
}


static void DeviceMatching_callback(void* inContext,
	IOReturn inResult,
	void *inSender,
	IOHIDDeviceRef inIOHIDDeviceRef)
{
	std::string name = [(NSString *)IOHIDDeviceGetProperty(inIOHIDDeviceRef,
		CFSTR(kIOHIDProductKey)) UTF8String];

	DeviceDebugPrint(inIOHIDDeviceRef);

	std::vector<ControllerInterface::Device*> *devices =
		(std::vector<ControllerInterface::Device*> *)inContext;

	// Add to the devices vector if it's of a type we want
	if (IOHIDDeviceConformsTo(inIOHIDDeviceRef,
		kHIDPage_GenericDesktop, kHIDUsage_GD_Keyboard))
		devices->push_back(new Keyboard(inIOHIDDeviceRef,
			name, kbd_name_counts[name]++));
#if 0
	else if (IOHIDDeviceConformsTo(inIOHIDDeviceRef,
		kHIDPage_GenericDesktop, kHIDUsage_GD_Mouse))
		devices->push_back(new Mouse(inIOHIDDeviceRef,
			name, mouse_name_counts[name++]));
#endif
	else 
		devices->push_back(new Joystick(inIOHIDDeviceRef,
			name, joy_name_counts[name]++));
}

void Init(std::vector<ControllerInterface::Device*>& devices)
{
	HIDManager = IOHIDManagerCreate(kCFAllocatorDefault,
		kIOHIDOptionsTypeNone);
	if (!HIDManager)
		NSLog(@"Failed to create HID Manager reference");

	IOHIDManagerSetDeviceMatching(HIDManager, NULL);

	// Callbacks for acquisition or loss of a matching device
	IOHIDManagerRegisterDeviceMatchingCallback(HIDManager,
		DeviceMatching_callback, (void *)&devices);

	// Match devices that are plugged in right now
	IOHIDManagerScheduleWithRunLoop(HIDManager,
		CFRunLoopGetCurrent(), OurRunLoop);
	if (IOHIDManagerOpen(HIDManager, kIOHIDOptionsTypeNone) !=
		kIOReturnSuccess)
		NSLog(@"Failed to open HID Manager");

	kbd_name_counts.clear();
	joy_name_counts.clear();

	// Wait while current devices are initialized
	while (CFRunLoopRunInMode(OurRunLoop, 0, TRUE) ==
		kCFRunLoopRunHandledSource) {};

	// Things should be configured now
	// Disable hotplugging and other scheduling
	IOHIDManagerRegisterDeviceMatchingCallback(HIDManager, NULL, NULL);
	IOHIDManagerUnscheduleFromRunLoop(HIDManager,
		CFRunLoopGetCurrent(), OurRunLoop);
}

void DeInit()
{
	// This closes all devices as well
	IOHIDManagerClose(HIDManager, kIOHIDOptionsTypeNone);
	CFRelease(HIDManager);
}


}
}
