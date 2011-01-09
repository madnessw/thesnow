/ /   C o p y r i g h t   ( C )   2 0 0 3   D o l p h i n   P r o j e c t .  
  
 / /   T h i s   p r o g r a m   i s   f r e e   s o f t w a r e :   y o u   c a n   r e d i s t r i b u t e   i t   a n d / o r   m o d i f y  
 / /   i t   u n d e r   t h e   t e r m s   o f   t h e   G N U   G e n e r a l   P u b l i c   L i c e n s e   a s   p u b l i s h e d   b y  
 / /   t h e   F r e e   S o f t w a r e   F o u n d a t i o n ,   v e r s i o n   2 . 0 .  
  
 / /   T h i s   p r o g r a m   i s   d i s t r i b u t e d   i n   t h e   h o p e   t h a t   i t   w i l l   b e   u s e f u l ,  
 / /   b u t   W I T H O U T   A N Y   W A R R A N T Y ;   w i t h o u t   e v e n   t h e   i m p l i e d   w a r r a n t y   o f  
 / /   M E R C H A N T A B I L I T Y   o r   F I T N E S S   F O R   A   P A R T I C U L A R   P U R P O S E .     S e e   t h e  
 / /   G N U   G e n e r a l   P u b l i c   L i c e n s e   2 . 0   f o r   m o r e   d e t a i l s .  
  
 / /   A   c o p y   o f   t h e   G P L   2 . 0   s h o u l d   h a v e   b e e n   i n c l u d e d   w i t h   t h e   p r o g r a m .  
 / /   I f   n o t ,   s e e   h t t p : / / w w w . g n u . o r g / l i c e n s e s /  
  
 / /   O f f i c i a l   S V N   r e p o s i t o r y   a n d   c o n t a c t   i n f o r m a t i o n   c a n   b e   f o u n d   a t  
 / /   h t t p : / / c o d e . g o o g l e . c o m / p / d o l p h i n - e m u /  
  
 # i f n d e f   _ _ I S O P R O P E R T I E S _ h _ _  
 # d e f i n e   _ _ I S O P R O P E R T I E S _ h _ _  
  
 # i n c l u d e   < w x / w x . h >  
 # i n c l u d e   < w x / s i z e r . h >  
 # i n c l u d e   < w x / f i l e p i c k e r . h >  
 # i n c l u d e   < w x / s t a t b m p . h >  
 # i n c l u d e   < w x / i m a g l i s t . h >  
 # i n c l u d e   < w x / f o n t m a p . h >  
 # i n c l u d e   < w x / t r e e c t r l . h >  
 # i n c l u d e   < w x / g b s i z e r . h >  
 # i n c l u d e   < w x / n o t e b o o k . h >  
 # i n c l u d e   < w x / m i m e t y p e . h >  
 # i n c l u d e   < s t r i n g >  
  
 # i n c l u d e   " I S O F i l e . h "  
 # i n c l u d e   " F i l e s y s t e m . h "  
 # i n c l u d e   " I n i F i l e . h "  
 # i n c l u d e   " P a t c h E n g i n e . h "  
 # i n c l u d e   " A c t i o n R e p l a y . h "  
 # i n c l u d e   " G e c k o C o d e D i a g . h "  
  
 c l a s s   C I S O P r o p e r t i e s   :   p u b l i c   w x D i a l o g  
 {  
 	 p u b l i c :  
  
 	 	 C I S O P r o p e r t i e s ( c o n s t   s t d : : s t r i n g   f i l e N a m e ,  
 	 	 	 w x W i n d o w *   p a r e n t ,  
 	 	 	 w x W i n d o w I D   i d   =   1 ,  
 	 	 	 c o n s t   w x S t r i n g &   t i t l e   =   _ ( " ^\'`" ) ,  
 	 	 	 c o n s t   w x P o i n t &   p o s   =   w x D e f a u l t P o s i t i o n ,  
 	 	 	 c o n s t   w x S i z e &   s i z e   =   w x D e f a u l t S i z e ,  
 	 	 	 l o n g   s t y l e   =   w x D E F A U L T _ D I A L O G _ S T Y L E | w x R E S I Z E _ B O R D E R ) ;  
 	 	 v i r t u a l   ~ C I S O P r o p e r t i e s ( ) ;  
  
 	 	 b o o l   b R e f r e s h L i s t ;  
  
 	 	 v o i d   A c t i o n R e p l a y L i s t _ L o a d ( ) ;  
 	 	 b o o l   S a v e G a m e C o n f i g ( ) ;  
  
 	 p r i v a t e :  
  
 	 	 D E C L A R E _ E V E N T _ T A B L E ( ) ;  
 	 	  
 	 	 w x S t a t i c B o x S i z e r   * s b G a m e C o n f i g ;  
 	 	 w x S t a t i c B o x S i z e r   * s b C o r e O v e r r i d e s ;  
 	 	 w x S t a t i c B o x S i z e r   * s b W i i O v e r r i d e s ;  
 	 	 w x S t a t i c B o x S i z e r   * s b V i d e o O v e r r i d e s ;  
 	 	 w x B o x S i z e r   * s E m u S t a t e ;  
 	 	 w x B o x S i z e r   * s P a t c h e s ;  
 	 	 w x B o x S i z e r   * s P a t c h B u t t o n s ;  
 	 	 w x B o x S i z e r   * s C h e a t s ;  
 	 	 w x B o x S i z e r   * s C h e a t B u t t o n s ;  
 	 	 w x S t a t i c B o x S i z e r   * s b I S O D e t a i l s ;  
 	 	 w x G r i d B a g S i z e r   * s I S O D e t a i l s ;  
 	 	 w x S t a t i c B o x S i z e r   * s b B a n n e r D e t a i l s ;  
 	 	 w x G r i d B a g S i z e r   * s B a n n e r D e t a i l s ;  
  
 	 	 w x B u t t o n   * m _ C l o s e ;  
  
 	 	 w x N o t e b o o k   * m _ N o t e b o o k ;  
 	 	 w x P a n e l   * m _ G a m e C o n f i g ;  
 	 	 w x P a n e l   * m _ P a t c h P a g e ;  
 	 	 w x P a n e l   * m _ C h e a t P a g e ;  
 	 	 w x P a n e l   * m _ I n f o r m a t i o n ;  
 	 	 w x P a n e l   * m _ F i l e s y s t e m ;  
  
 	 	 w x S t a t i c T e x t   * O v e r r i d e T e x t ;  
 	 	 / /   C o r e  
 	 	 w x C h e c k B o x   * C P U T h r e a d ,   * S k i p I d l e ,   * M M U ,   * M M U B A T ,   * T L B H a c k ;  
 	 	 w x C h e c k B o x   * A l t e r n a t e R F I ,   * F a s t D i s c S p e e d ,   * B l o c k M e r g i n g ;  
 	 	 / /   W i i  
 	 	 w x C h e c k B o x   * E n a b l e P r o g r e s s i v e S c a n ,   * E n a b l e W i d e S c r e e n ;  
 	 	 / /   V i d e o  
 	 	 w x C h e c k B o x   * F o r c e F i l t e r i n g ,  
 	 	 	 * E F B C o p y E n a b l e ,   * E F B A c c e s s E n a b l e ,   * E F B T o T e x t u r e E n a b l e ,  
 	 	 	 * S a f e T e x t u r e C a c h e ,   * D s t A l p h a P a s s ,   * U s e X F B ,   * U s e Z T P S p e e d u p H a c k ,  
 	 	 	 * D L i s t C a c h e ;  
 	 	 w x S t a t i c T e x t   * H a c k t e x t ;  
 	 	 w x A r r a y S t r i n g   a r r a y S t r i n g F o r _ H a c k ;  
 	 	 w x C h o i c e   * H a c k ;  
  
 	 	 w x B u t t o n   * E d i t C o n f i g ;  
 	 	 w x S t a t i c T e x t   * E m u S t a t e T e x t ;  
 	 	 w x A r r a y S t r i n g   a r r a y S t r i n g F o r _ E m u S t a t e ;  
 	 	 w x C h o i c e   * E m u S t a t e ;  
 	 	 w x T e x t C t r l   * E m u I s s u e s ;  
 	 	 w x A r r a y S t r i n g   a r r a y S t r i n g F o r _ P a t c h e s ;  
 	 	 w x C h e c k L i s t B o x   * P a t c h e s ;  
 	 	 w x B u t t o n   * E d i t P a t c h ;  
 	 	 w x B u t t o n   * A d d P a t c h ;  
 	 	 w x B u t t o n   * R e m o v e P a t c h ;  
 	 	 w x A r r a y S t r i n g   a r r a y S t r i n g F o r _ C h e a t s ;  
 	 	 w x C h e c k L i s t B o x   * C h e a t s ;  
 	 	 w x B u t t o n   * E d i t C h e a t ;  
 	 	 w x B u t t o n   * A d d C h e a t ;  
 	 	 w x B u t t o n   * R e m o v e C h e a t ;  
 	 	 w x A r r a y S t r i n g   a r r a y S t r i n g F o r _ S p e e d h a c k s ;  
 	 	 w x C h e c k L i s t B o x   * S p e e d h a c k s ;  
 	 	 w x B u t t o n   * E d i t S p e e d h a c k ;  
 	 	 w x B u t t o n   * A d d S p e e d h a c k ;  
 	 	 w x B u t t o n   * R e m o v e S p e e d h a c k ;  
  
 	 	 w x S t a t i c T e x t   * m _ N a m e T e x t ;  
 	 	 w x S t a t i c T e x t   * m _ G a m e I D T e x t ;  
 	 	 w x S t a t i c T e x t   * m _ C o u n t r y T e x t ;  
 	 	 w x S t a t i c T e x t   * m _ M a k e r I D T e x t ;  
 	 	 w x S t a t i c T e x t   * m _ D a t e T e x t ;  
 	 	 w x S t a t i c T e x t   * m _ F S T T e x t ;  
 	 	 w x S t a t i c T e x t   * m _ L a n g T e x t ;  
 	 	 w x S t a t i c T e x t   * m _ S h o r t T e x t ;  
 	 	 w x S t a t i c T e x t   * m _ M a k e r T e x t ;  
 	 	 w x S t a t i c T e x t   * m _ C o m m e n t T e x t ;  
 	 	 w x S t a t i c T e x t   * m _ B a n n e r T e x t ;  
 	 	 w x T e x t C t r l   * m _ N a m e ;  
 	 	 w x T e x t C t r l   * m _ G a m e I D ;  
 	 	 w x T e x t C t r l   * m _ C o u n t r y ;  
 	 	 w x T e x t C t r l   * m _ M a k e r I D ;  
 	 	 w x T e x t C t r l   * m _ D a t e ;  
 	 	 w x T e x t C t r l   * m _ F S T ;  
 	 	 w x A r r a y S t r i n g   a r r a y S t r i n g F o r _ L a n g ;  
 	 	 w x C h o i c e   * m _ L a n g ;  
 	 	 w x T e x t C t r l   * m _ S h o r t N a m e ;  
 	 	 w x T e x t C t r l   * m _ M a k e r ;  
 	 	 w x T e x t C t r l   * m _ C o m m e n t ;  
 	 	 w x S t a t i c B i t m a p   * m _ B a n n e r ;  
  
 	 	 w x T r e e C t r l   * m _ T r e e c t r l ;  
 	 	 w x T r e e I t e m I d   R o o t I d ;  
 	 	 w x I m a g e L i s t   * m _ i c o n L i s t ;  
  
 	 	 G e c k o : : C o d e C o n f i g P a n e l   * m _ g e c k o c o d e _ p a n e l ;  
  
 	 	 e n u m  
 	 	 {  
 	 	 	 I D _ C L O S E   =   1 0 0 0 ,  
 	 	 	 I D _ T R E E C T R L ,  
  
 	 	 	 I D _ N O T E B O O K ,  
 	 	 	 I D _ G A M E C O N F I G ,  
 	 	 	 I D _ P A T C H _ P A G E ,  
 	 	 	 I D _ A R C O D E _ P A G E ,  
 	 	 	 I D _ S P E E D H A C K _ P A G E ,  
 	 	 	 I D _ I N F O R M A T I O N ,  
 	 	 	 I D _ F I L E S Y S T E M ,  
  
 	 	 	 I D _ O V E R R I D E _ T E X T ,  
 	 	 	 I D _ U S E D U A L C O R E ,  
 	 	 	 I D _ I D L E S K I P ,  
 	 	 	 I D _ M M U ,  
 	 	 	 I D _ M M U B A T ,  
 	 	 	 I D _ T L B H A C K ,  
 	 	 	 I D _ R F I ,  
 	 	 	 I D _ D I S C S P E E D ,  
 	 	 	 I D _ M E R G E B L O C K S ,  
 	 	 	 I D _ F O R C E F I L T E R I N G ,  
 	 	 	 I D _ E F B C O P Y E N A B L E ,  
 	 	 	 I D _ E F B A C C E S S E N A B L E ,  
 	 	 	 I D _ E F B T O T E X T U R E E N A B L E ,  
 	 	 	 I D _ S A F E T E X T U R E C A C H E ,  
 	 	 	 I D _ D S T A L P H A P A S S ,  
 	 	 	 I D _ U S E X F B ,  
 	 	 	 I D _ Z T P _ S P E E D U P ,  
 	 	 	 I D _ D L I S T C A C H E ,  
 	 	 	 I D _ H A C K _ T E X T ,  
 	 	 	 I D _ H A C K ,  
 	 	 	 I D _ E N A B L E P R O G R E S S I V E S C A N ,  
 	 	 	 I D _ E N A B L E W I D E S C R E E N ,  
 	 	 	 I D _ E D I T C O N F I G ,  
 	 	 	 I D _ E M U S T A T E _ T E X T ,  
 	 	 	 I D _ E M U S T A T E ,  
 	 	 	 I D _ E M U I S S U E S _ T E X T ,  
 	 	 	 I D _ E M U _ I S S U E S ,  
 	 	 	 I D _ P A T C H E S _ L I S T ,  
 	 	 	 I D _ E D I T P A T C H ,  
 	 	 	 I D _ A D D P A T C H ,  
 	 	 	 I D _ R E M O V E P A T C H ,  
 	 	 	 I D _ C H E A T S _ L I S T ,  
 	 	 	 I D _ E D I T C H E A T ,  
 	 	 	 I D _ A D D C H E A T ,  
 	 	 	 I D _ R E M O V E C H E A T ,  
 	 	 	  
 	 	 	 I D _ N A M E _ T E X T ,  
 	 	 	 I D _ G A M E I D _ T E X T ,  
 	 	 	 I D _ C O U N T R Y _ T E X T ,  
 	 	 	 I D _ M A K E R I D _ T E X T ,  
 	 	 	 I D _ D A T E _ T E X T ,  
 	 	 	 I D _ F S T _ T E X T ,  
 	 	 	 I D _ V E R S I O N _ T E X T ,  
 	 	 	 I D _ L A N G _ T E X T ,  
 	 	 	 I D _ S H O R T N A M E _ T E X T ,  
 	 	 	 I D _ L O N G N A M E _ T E X T ,  
 	 	 	 I D _ M A K E R _ T E X T ,  
 	 	 	 I D _ C O M M E N T _ T E X T ,  
 	 	 	 I D _ B A N N E R _ T E X T ,  
 	 	 	  
 	 	 	 I D _ N A M E ,  
 	 	 	 I D _ G A M E I D ,  
 	 	 	 I D _ C O U N T R Y ,  
 	 	 	 I D _ M A K E R I D ,  
 	 	 	 I D _ D A T E ,  
 	 	 	 I D _ F S T ,  
 	 	 	 I D _ V E R S I O N ,  
 	 	 	 I D _ L A N G ,  
 	 	 	 I D _ S H O R T N A M E ,  
 	 	 	 I D _ L O N G N A M E ,  
 	 	 	 I D _ M A K E R ,  
 	 	 	 I D _ C O M M E N T ,  
 	 	 	 I D _ B A N N E R ,  
 	 	 	 I D M _ E X T R A C T D I R ,  
 	 	 	 I D M _ E X T R A C T A L L ,  
 	 	 	 I D M _ E X T R A C T F I L E ,  
 	 	 	 I D M _ E X T R A C T A P P L O A D E R ,  
 	 	 	 I D M _ E X T R A C T D O L ,  
 	 	 	 I D M _ B N R S A V E A S  
 	 	 } ;  
  
 	 	 v o i d   C r e a t e G U I C o n t r o l s ( b o o l ) ;  
 	 	 v o i d   O n C l o s e ( w x C l o s e E v e n t &   e v e n t ) ;  
 	 	 v o i d   O n C l o s e C l i c k ( w x C o m m a n d E v e n t &   e v e n t ) ;  
 	 	 v o i d   O n E d i t C o n f i g ( w x C o m m a n d E v e n t &   e v e n t ) ;  
 	 	 v o i d   L i s t S e l e c t i o n C h a n g e d ( w x C o m m a n d E v e n t &   e v e n t ) ;  
 	 	 v o i d   P a t c h B u t t o n C l i c k e d ( w x C o m m a n d E v e n t &   e v e n t ) ;  
 	 	 v o i d   A c t i o n R e p l a y B u t t o n C l i c k e d ( w x C o m m a n d E v e n t &   e v e n t ) ;  
 	 	 v o i d   R i g h t C l i c k O n B a n n e r ( w x M o u s e E v e n t &   e v e n t ) ;  
 	 	 v o i d   O n B a n n e r I m a g e S a v e ( w x C o m m a n d E v e n t &   e v e n t ) ;  
 	 	 v o i d   O n R i g h t C l i c k O n T r e e ( w x T r e e E v e n t &   e v e n t ) ;  
 	 	 v o i d   O n E x t r a c t F i l e ( w x C o m m a n d E v e n t &   e v e n t ) ;  
 	 	 v o i d   O n E x t r a c t D i r ( w x C o m m a n d E v e n t &   e v e n t ) ;  
 	 	 v o i d   O n E x t r a c t D a t a F r o m H e a d e r ( w x C o m m a n d E v e n t &   e v e n t ) ;  
 	 	 v o i d   S e t R e f r e s h ( w x C o m m a n d E v e n t &   e v e n t ) ;  
 	 	 v o i d   O n C h a n g e B a n n e r L a n g ( w x C o m m a n d E v e n t &   e v e n t ) ;  
  
 	 	 G a m e L i s t I t e m   * O p e n G a m e L i s t I t e m ;  
  
 	 	 s t d : : v e c t o r < c o n s t   D i s c I O : : S F i l e I n f o   * >   G C F i l e s ;  
 	 	 t y p e d e f   s t d : : v e c t o r < c o n s t   D i s c I O : : S F i l e I n f o   * > : : i t e r a t o r   f i l e I t e r ;  
  
 	 	 s i z e _ t   C r e a t e D i r e c t o r y T r e e ( w x T r e e I t e m I d &   p a r e n t ,  
 	 	 	 	 	 	 	 	   s t d : : v e c t o r < c o n s t   D i s c I O : : S F i l e I n f o * >   f i l e I n f o s ,  
 	 	 	 	 	 	 	 	   c o n s t   s i z e _ t   _ F i r s t I n d e x ,    
 	 	 	 	 	 	 	 	   c o n s t   s i z e _ t   _ L a s t I n d e x ) ;  
 	 	 v o i d   E x p o r t D i r ( c o n s t   c h a r *   _ r F u l l P a t h ,   c o n s t   c h a r *   _ r E x p o r t F i l e n a m e ,  
 	 	 	 	 	 	 	 	   c o n s t   i n t   p a r t i t i o n N u m   =   0 ) ;  
  
 	 	 I n i F i l e   G a m e I n i ;  
 	 	 s t d : : s t r i n g   G a m e I n i F i l e ;  
  
 	 	 v o i d   L o a d G a m e C o n f i g ( ) ;  
 	 	 v o i d   P a t c h L i s t _ L o a d ( ) ;  
 	 	 v o i d   P a t c h L i s t _ S a v e ( ) ;  
 	 	 v o i d   A c t i o n R e p l a y L i s t _ S a v e ( ) ;  
 	 	 v o i d   C h a n g e B a n n e r D e t a i l s ( i n t   l a n g ) ;  
 } ;  
 # e n d i f  
 