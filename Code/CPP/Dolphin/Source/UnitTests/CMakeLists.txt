set(LIBS	core
		common)

set(SRCS	AudioJitTests.cpp
		DSPJitTester.cpp
		UnitTests.cpp)

add_executable(tester ${SRCS})
target_link_libraries(tester ${LIBS})
#install(TARGETS ${EXEGUI} RUNTIME DESTINATION ${bindir})
