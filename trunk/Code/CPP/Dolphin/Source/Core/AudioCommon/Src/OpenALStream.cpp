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
 # i n c l u d e   " a l d l i s t . h "  
 # i n c l u d e   " O p e n A L S t r e a m . h "  
  
 # i f   d e f i n e d   H A V E _ O P E N A L   & &   H A V E _ O P E N A L  
  
 / /  
 / /   A y u a n X :   S p e c   s a y s   O p e n A L 1 . 1   i s   t h r e a d   s a f e   a l r e a d y  
 / /  
 b o o l   O p e n A L S t r e a m : : S t a r t ( )  
 {  
 	 A L D e v i c e L i s t   * p D e v i c e L i s t   =   N U L L ;  
 	 A L C c o n t e x t   * p C o n t e x t   =   N U L L ;  
 	 A L C d e v i c e   * p D e v i c e   =   N U L L ;  
 	 b o o l   b R e t u r n   =   f a l s e ;  
  
 	 s o u n d S y n c E v e n t . I n i t ( ) ;  
  
 	 p D e v i c e L i s t   =   n e w   A L D e v i c e L i s t ( ) ;  
 	 i f   ( ( p D e v i c e L i s t )   & &   ( p D e v i c e L i s t - > G e t N u m D e v i c e s ( ) ) )  
 	 {  
 	 	 c h a r   * d e f D e v N a m e   =   p D e v i c e L i s t - >   \  
 	 	 	 G e t D e v i c e N a m e ( p D e v i c e L i s t - > G e t D e f a u l t D e v i c e ( ) ) ;  
 	 	 p D e v i c e   =   a l c O p e n D e v i c e ( d e f D e v N a m e ) ;  
 	 	 i f   ( p D e v i c e )  
 	 	 {  
 	 	 	 p C o n t e x t   =   a l c C r e a t e C o n t e x t ( p D e v i c e ,   N U L L ) ;  
 	 	 	 i f   ( p C o n t e x t )  
 	 	 	 {  
 	 	 	 	 a l c M a k e C o n t e x t C u r r e n t ( p C o n t e x t ) ;  
 	 	 	 	 t h r e a d   =   n e w   C o m m o n : : T h r e a d (  
 	 	 	 	 	 O p e n A L S t r e a m : : T h r e a d F u n c ,   ( v o i d   * ) t h i s ) ;  
 	 	 	 	 b R e t u r n   =   t r u e ;  
 	 	 	 }  
 	 	 	 e l s e  
 	 	 	 {  
 	 	 	 	 a l c C l o s e D e v i c e ( p D e v i c e ) ;  
 	 	 	 	 P a n i c A l e r t ( " O p e n A L :   N��R�^��Y  % s   
NN�e" ,   d e f D e v N a m e ) ;  
 	 	 	 }  
 	 	 }  
 	 	 e l s e  
 	 	 {  
 	 	 	 P a n i c A l e r t ( " O p e n A L :   N��Sb _��Y  % s " ,   d e f D e v N a m e ) ;  
 	 	 }  
 	 	 d e l e t e   p D e v i c e L i s t ;  
 	 }  
 	 e l s e  
 	 {  
 	 	 P a n i c A l e r t ( " O p e n A L :   N��~b0R�X󗾋Y" ) ;  
 	 }  
  
 	 r e t u r n   b R e t u r n ;  
 }  
  
 v o i d   O p e n A L S t r e a m : : S t o p ( )  
 {  
 	 t h r e a d D a t a   =   1 ;  
 	 / /   k i c k   t h e   t h r e a d   i f   i t ' s   w a i t i n g  
 	 s o u n d S y n c E v e n t . S e t ( ) ;  
  
 	 d e l e t e   t h r e a d ;  
 	 t h r e a d   =   N U L L ;  
  
 	 a l S o u r c e S t o p ( u i S o u r c e ) ;  
 	 a l S o u r c e i ( u i S o u r c e ,   A L _ B U F F E R ,   0 ) ;  
  
 	 / /   C l e a n   u p   b u f f e r s   a n d   s o u r c e s  
 	 a l D e l e t e S o u r c e s ( 1 ,   & u i S o u r c e ) ;  
 	 u i S o u r c e   =   0 ;  
 	 a l D e l e t e B u f f e r s ( O A L _ N U M _ B U F F E R S ,   u i B u f f e r s ) ;  
  
 	 A L C c o n t e x t   * p C o n t e x t   =   a l c G e t C u r r e n t C o n t e x t ( ) ;  
 	 A L C d e v i c e   * p D e v i c e   =   a l c G e t C o n t e x t s D e v i c e ( p C o n t e x t ) ;  
  
 	 a l c M a k e C o n t e x t C u r r e n t ( N U L L ) ;  
 	 a l c D e s t r o y C o n t e x t ( p C o n t e x t ) ;  
 	 a l c C l o s e D e v i c e ( p D e v i c e ) ;  
  
 	 s o u n d S y n c E v e n t . S h u t d o w n ( ) ;  
 }  
  
 v o i d   O p e n A L S t r e a m : : S e t V o l u m e ( i n t   v o l u m e )  
 {  
 	 f V o l u m e   =   ( f l o a t ) v o l u m e   /   1 0 0 . 0 f ;  
  
 	 i f   ( u i S o u r c e )  
 	 	 a l S o u r c e f ( u i S o u r c e ,   A L _ G A I N ,   f V o l u m e ) ;    
 }  
  
 v o i d   O p e n A L S t r e a m : : U p d a t e ( )  
 {  
 	 s o u n d S y n c E v e n t . S e t ( ) ;  
 }  
  
 v o i d   O p e n A L S t r e a m : : C l e a r ( b o o l   m u t e )  
 {  
 	 m _ m u t e d   =   m u t e ;  
  
 	 i f ( m _ m u t e d )  
 	 {  
 	 	 a l S o u r c e S t o p ( u i S o u r c e ) ;  
 	 }  
 	 e l s e  
 	 {  
 	 	 a l S o u r c e P l a y ( u i S o u r c e ) ;  
 	 }  
 }  
  
 T H R E A D _ R E T U R N   O p e n A L S t r e a m : : T h r e a d F u n c ( v o i d *   a r g s )  
 {  
 	 ( r e i n t e r p r e t _ c a s t < O p e n A L S t r e a m   * > ( a r g s ) ) - > S o u n d L o o p ( ) ;  
 	 r e t u r n   0 ;  
 }  
  
 v o i d   O p e n A L S t r e a m : : S o u n d L o o p ( )  
 {  
 	 A L e n u m   e r r ;  
 	 u 3 2   u l F r e q u e n c y   =   m _ m i x e r - > G e t S a m p l e R a t e ( ) ;  
  
 	 m e m s e t ( u i B u f f e r s ,   0 ,   O A L _ N U M _ B U F F E R S   *   s i z e o f ( A L u i n t ) ) ;  
 	 u i S o u r c e   =   0 ;  
  
 	 / /   G e n e r a t e   s o m e   A L   B u f f e r s   f o r   s t r e a m i n g  
 	 a l G e n B u f f e r s ( O A L _ N U M _ B U F F E R S ,   ( A L u i n t   * ) u i B u f f e r s ) ;  
 	 / /   G e n e r a t e   a   S o u r c e   t o   p l a y b a c k   t h e   B u f f e r s  
 	 a l G e n S o u r c e s ( 1 ,   & u i S o u r c e ) ;  
  
 	 / /   S h o r t   S i l e n c e  
 	 m e m s e t ( r e a l t i m e B u f f e r ,   0 ,   O A L _ M A X _ S A M P L E S   *   4 ) ;  
 	 f o r   ( i n t   i   =   0 ;   i   <   O A L _ N U M _ B U F F E R S ;   i + + )  
 	 	 a l B u f f e r D a t a ( u i B u f f e r s [ i ] ,   A L _ F O R M A T _ S T E R E O 1 6 ,   r e a l t i m e B u f f e r ,   O A L _ M A X _ S A M P L E S ,   u l F r e q u e n c y ) ;  
 	 a l S o u r c e Q u e u e B u f f e r s ( u i S o u r c e ,   O A L _ N U M _ B U F F E R S ,   u i B u f f e r s ) ;  
 	 a l S o u r c e P l a y ( u i S o u r c e ) ;  
 	  
 	 / /   S e t   t h e   d e f a u l t   s o u n d   v o l u m e   a s   s a v e d   i n   t h e   c o n f i g   f i l e .  
 	 a l S o u r c e f ( u i S o u r c e ,   A L _ G A I N ,   f V o l u m e ) ;    
  
 	 e r r   =   a l G e t E r r o r ( ) ;  
 	 / /   T O D O :   E r r o r   h a n d l i n g  
  
 	 A L i n t   i B u f f e r s F i l l e d   =   0 ;  
 	 A L i n t   i B u f f e r s P r o c e s s e d   =   0 ;  
 	 A L u i n t   u i B u f f e r T e m p [ O A L _ N U M _ B U F F E R S ]   =   { 0 } ;  
  
 	 w h i l e   ( ! t h r e a d D a t a )    
 	 {  
 	 	 i f   ( i B u f f e r s P r o c e s s e d   = =   i B u f f e r s F i l l e d )  
 	 	 {  
 	 	 	 a l G e t S o u r c e i ( u i S o u r c e ,   A L _ B U F F E R S _ P R O C E S S E D ,   & i B u f f e r s P r o c e s s e d ) ;  
 	 	 	 i B u f f e r s F i l l e d   =   0 ;  
 	 	 }  
  
 	 	 u n s i g n e d   i n t   n u m S a m p l e s   =   m _ m i x e r - > G e t N u m S a m p l e s ( ) ;  
  
 	 	 i f   ( i B u f f e r s P r o c e s s e d   & &   ( n u m S a m p l e s   > =   O A L _ T H R E S H O L D ) )  
 	 	 {  
 	 	 	 n u m S a m p l e s   =   ( n u m S a m p l e s   >   O A L _ M A X _ S A M P L E S )   ?   O A L _ M A X _ S A M P L E S   :   n u m S a m p l e s ;  
 	 	 	 / /   R e m o v e   t h e   B u f f e r   f r o m   t h e   Q u e u e .     ( u i B u f f e r   c o n t a i n s   t h e   B u f f e r   I D   f o r   t h e   u n q u e u e d   B u f f e r )  
 	 	 	 i f   ( i B u f f e r s F i l l e d   = =   0 )  
 	 	 	 	 a l S o u r c e U n q u e u e B u f f e r s ( u i S o u r c e ,   i B u f f e r s P r o c e s s e d ,   u i B u f f e r T e m p ) ;  
  
 	 	 	 m _ m i x e r - > M i x ( r e a l t i m e B u f f e r ,   n u m S a m p l e s ) ;  
 	 	 	 a l B u f f e r D a t a ( u i B u f f e r T e m p [ i B u f f e r s F i l l e d ] ,   A L _ F O R M A T _ S T E R E O 1 6 ,   r e a l t i m e B u f f e r ,   n u m S a m p l e s   *   4 ,   u l F r e q u e n c y ) ;  
 	 	 	 a l S o u r c e Q u e u e B u f f e r s ( u i S o u r c e ,   1 ,   & u i B u f f e r T e m p [ i B u f f e r s F i l l e d ] ) ;  
 	 	 	 i B u f f e r s F i l l e d + + ;  
  
 	 	 	 i f   ( i B u f f e r s F i l l e d   = =   O A L _ N U M _ B U F F E R S )  
 	 	 	 	 a l S o u r c e P l a y ( u i S o u r c e ) ;  
 	 	 }  
 	 	 e l s e   i f   ( n u m S a m p l e s   > =   O A L _ T H R E S H O L D )  
 	 	 {  
 	 	 	 A L i n t   s t a t e   =   0 ;  
 	 	 	 a l G e t S o u r c e i ( u i S o u r c e ,   A L _ S O U R C E _ S T A T E ,   & s t a t e ) ;  
 	 	 	 i f   ( s t a t e   = =   A L _ S T O P P E D )  
 	 	 	 	 a l S o u r c e P l a y ( u i S o u r c e ) ;  
 	 	 }  
 	 	 s o u n d S y n c E v e n t . W a i t ( ) ;  
 	 }  
 }  
  
 # e n d i f   / / H A V E _ O P E N A L  
  
 