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
 # i n c l u d e   " M e m c a r d M a n a g e r . h "  
 # i n c l u d e   " C o m m o n . h "  
 # i n c l u d e   " w x / m s t r e a m . h "  
  
 # d e f i n e   A R R O W S   s l o t   ?   _ T ( " " )   :   A R R O W [ s l o t ] ,   s l o t   ?   A R R O W [ s l o t ]   :   _ T ( " " )  
  
 c o n s t   u 8   h d r [ ]   =   {  
 0 x 4 2 , 0 x 4 D ,  
 0 x 3 8 , 0 x 3 0 , 0 x 0 0 , 0 x 0 0 ,  
 0 x 0 0 , 0 x 0 0 , 0 x 0 0 , 0 x 0 0 ,  
 0 x 3 6 , 0 x 0 0 , 0 x 0 0 , 0 x 0 0 ,  
 0 x 2 8 , 0 x 0 0 , 0 x 0 0 , 0 x 0 0 ,  
 0 x 2 0 , 0 x 0 0 , 0 x 0 0 , 0 x 0 0 ,   / / W  
 0 x 2 0 , 0 x 0 0 , 0 x 0 0 , 0 x 0 0 ,   / / H  
 0 x 0 1 , 0 x 0 0 ,  
 0 x 2 0 , 0 x 0 0 ,  
 0 x 0 0 , 0 x 0 0 , 0 x 0 0 , 0 x 0 0 ,  
 0 x 0 2 , 0 x 3 0 , 0 x 0 0 , 0 x 0 0 ,   / / d a t a   s i z e  
 0 x 1 2 , 0 x 0 B , 0 x 0 0 , 0 x 0 0 ,  
 0 x 1 2 , 0 x 0 B , 0 x 0 0 , 0 x 0 0 ,  
 0 x 0 0 , 0 x 0 0 , 0 x 0 0 , 0 x 0 0 ,  
 0 x 0 0 , 0 x 0 0 , 0 x 0 0 , 0 x 0 0  
 } ;  
  
 w x B i t m a p   w x B i t m a p F r o m M e m o r y R G B A ( c o n s t   u n s i g n e d   c h a r *   d a t a ,   i n t   w i d t h ,   i n t   h e i g h t )  
 {  
 	 i n t   s t r i d e   =   ( 4 * w i d t h ) ;  
  
 	 i n t   b y t e s   =   ( s t r i d e * h e i g h t )   +   s i z e o f ( h d r ) ;  
  
 	 b y t e s   =   ( b y t e s + 3 ) & ( ~ 3 ) ;  
  
 	 u 8   * p d a t a   =   n e w   u 8 [ b y t e s ] ;  
  
 	 m e m s e t ( p d a t a , 0 , b y t e s ) ;  
 	 m e m c p y ( p d a t a , h d r , s i z e o f ( h d r ) ) ;  
  
 	 u 8   * p i x e l D a t a   =   p d a t a   +   s i z e o f ( h d r ) ;  
  
 	 f o r   ( i n t   y = 0 ; y < h e i g h t ; y + + )  
 	 {  
 	 	 m e m c p y ( p i x e l D a t a + y * s t r i d e , d a t a + ( h e i g h t - y - 1 ) * s t r i d e , s t r i d e ) ;  
 	 }  
  
 	 * ( i n t * ) ( p d a t a + 1 8 )   =   w i d t h ;  
 	 * ( i n t * ) ( p d a t a + 2 2 )   =   h e i g h t ;  
 	 * ( u 3 2 * ) ( p d a t a + 3 4 )   =   b y t e s - s i z e o f ( h d r ) ;  
  
 	 w x M e m o r y I n p u t S t r e a m   i s ( p d a t a ,   b y t e s ) ;  
 	 w x B i t m a p   m a p ( w x I m a g e ( i s ,   w x B I T M A P _ T Y P E _ B M P ,   - 1 ) ,   - 1 ) ;  
  
 	 d e l e t e   [ ]   p d a t a ;  
  
 	 r e t u r n   m a p ;  
 }  
  
 B E G I N _ E V E N T _ T A B L E ( C M e m c a r d M a n a g e r ,   w x D i a l o g )  
 	 E V T _ C L O S E ( C M e m c a r d M a n a g e r : : O n C l o s e )  
  
 	 E V T _ B U T T O N ( I D _ C O P Y F R O M _ A , C M e m c a r d M a n a g e r : : C o p y D e l e t e C l i c k )  
 	 E V T _ B U T T O N ( I D _ C O P Y F R O M _ B , C M e m c a r d M a n a g e r : : C o p y D e l e t e C l i c k )  
 	 E V T _ B U T T O N ( I D _ D E L E T E _ A , C M e m c a r d M a n a g e r : : C o p y D e l e t e C l i c k )  
 	 E V T _ B U T T O N ( I D _ D E L E T E _ B , C M e m c a r d M a n a g e r : : C o p y D e l e t e C l i c k )  
 	 E V T _ B U T T O N ( I D _ S A V E I M P O R T _ B , C M e m c a r d M a n a g e r : : C o p y D e l e t e C l i c k )  
 	 E V T _ B U T T O N ( I D _ S A V E E X P O R T _ B , C M e m c a r d M a n a g e r : : C o p y D e l e t e C l i c k )  
 	 E V T _ B U T T O N ( I D _ S A V E I M P O R T _ A , C M e m c a r d M a n a g e r : : C o p y D e l e t e C l i c k )  
 	 E V T _ B U T T O N ( I D _ S A V E E X P O R T _ A , C M e m c a r d M a n a g e r : : C o p y D e l e t e C l i c k )  
 	 E V T _ B U T T O N ( I D _ C O N V E R T T O G C I , C M e m c a r d M a n a g e r : : C o p y D e l e t e C l i c k )  
 	 E V T _ B U T T O N ( I D _ P R E V P A G E _ A ,   C M e m c a r d M a n a g e r : : O n P a g e C h a n g e )  
 	 E V T _ B U T T O N ( I D _ N E X T P A G E _ A ,   C M e m c a r d M a n a g e r : : O n P a g e C h a n g e )  
 	 E V T _ B U T T O N ( I D _ P R E V P A G E _ B ,   C M e m c a r d M a n a g e r : : O n P a g e C h a n g e )  
 	 E V T _ B U T T O N ( I D _ N E X T P A G E _ B ,   C M e m c a r d M a n a g e r : : O n P a g e C h a n g e )  
  
 	 E V T _ F I L E P I C K E R _ C H A N G E D ( I D _ M E M C A R D P A T H _ A , C M e m c a r d M a n a g e r : : O n P a t h C h a n g e )  
 	 E V T _ F I L E P I C K E R _ C H A N G E D ( I D _ M E M C A R D P A T H _ B , C M e m c a r d M a n a g e r : : O n P a t h C h a n g e )  
  
 	 E V T _ M E N U _ R A N G E ( I D _ M E M C A R D P A T H _ A ,   I D _ U S E P A G E S ,   C M e m c a r d M a n a g e r : : O n M e n u C h a n g e )  
 	 E V T _ M E N U _ R A N G E ( I D _ C O P Y F R O M _ A ,   I D _ C O N V E R T T O G C I ,   C M e m c a r d M a n a g e r : : C o p y D e l e t e C l i c k )  
 	 E V T _ M E N U _ R A N G E ( I D _ N E X T P A G E _ A ,   I D _ P R E V P A G E _ B ,   C M e m c a r d M a n a g e r : : O n P a g e C h a n g e )  
 	 E V T _ M E N U _ R A N G E ( C O L U M N _ B A N N E R ,   N U M B E R _ O F _ C O L U M N ,   C M e m c a r d M a n a g e r : : O n M e n u C h a n g e )  
 E N D _ E V E N T _ T A B L E ( )  
  
 B E G I N _ E V E N T _ T A B L E ( C M e m c a r d M a n a g e r : : C M e m c a r d L i s t C t r l ,   w x L i s t C t r l )  
 	 E V T _ R I G H T _ D O W N ( C M e m c a r d M a n a g e r : : C M e m c a r d L i s t C t r l : : O n R i g h t C l i c k )  
 E N D _ E V E N T _ T A B L E ( )  
  
 C M e m c a r d M a n a g e r : : C M e m c a r d M a n a g e r ( w x W i n d o w *   p a r e n t ,   w x W i n d o w I D   i d ,   c o n s t   w x S t r i n g &   t i t l e ,   c o n s t   w x P o i n t &   p o s i t i o n ,   c o n s t   w x S i z e &   s i z e ,   l o n g   s t y l e )  
 	 :   w x D i a l o g ( p a r e n t ,   i d ,   t i t l e ,   p o s i t i o n ,   s i z e ,   s t y l e )  
 {  
 	 m e m o r y C a r d [ S L O T _ A ] = N U L L ;  
 	 m e m o r y C a r d [ S L O T _ B ] = N U L L ;  
  
 	 i f   ( ! L o a d S e t t i n g s ( ) )  
 	 {  
 	 	 i t e m s P e r P a g e   =   1 6 ;  
 	 	 m c m S e t t i n g s . u s e P a g e s   =   t r u e ;  
 	 	 f o r   ( i n t   i   =   0 ;   i   <   N U M B E R _ O F _ C O L U M N ;   i + + )  
 	 	 {  
 	 	 	 m c m S e t t i n g s . c o l u m n [ i ]   =   ( i   < =   C O L U M N _ F I R S T B L O C K ) ?   t r u e : f a l s e ;  
 	 	 }  
 	 }  
 	 m a x P a g e s   =   ( 1 2 8   /   i t e m s P e r P a g e )   -   1 ;  
 	 C r e a t e G U I C o n t r o l s ( ) ;  
 }  
  
 C M e m c a r d M a n a g e r : : ~ C M e m c a r d M a n a g e r ( )  
 {  
 	 i f   ( m e m o r y C a r d [ S L O T _ A ] )  
 	 {  
 	 	 d e l e t e   m e m o r y C a r d [ S L O T _ A ] ;  
 	 	 m e m o r y C a r d [ S L O T _ A ]   =   N U L L ;  
 	 }  
 	 i f   ( m e m o r y C a r d [ S L O T _ B ] )  
 	 {  
 	 	 d e l e t e   m e m o r y C a r d [ S L O T _ B ] ;  
 	 	 m e m o r y C a r d [ S L O T _ B ]   =   N U L L ;  
 	 }  
 	 S a v e S e t t i n g s ( ) ;  
 }  
  
 b o o l   C M e m c a r d M a n a g e r : : L o a d S e t t i n g s ( )  
 {  
 	 i f   ( M e m c a r d M a n a g e r I n i . L o a d ( F i l e : : G e t U s e r P a t h ( F _ D O L P H I N C O N F I G _ I D X ) ) )  
 	 {  
 	 	 i n i M e m c a r d S e c t i o n   =   M e m c a r d M a n a g e r I n i . G e t O r C r e a t e S e c t i o n ( " M e m c a r d M a n a g e r " ) ;  
 	 	 i n i M e m c a r d S e c t i o n - > G e t ( " I t e m s   p e r   p a g e " ,     & i t e m s P e r P a g e ,   1 6 ) ;  
 	 	 i n i M e m c a r d S e c t i o n - > G e t ( " D e f a u l t M e m c a r d A " ,   & ( D e f a u l t M e m c a r d [ S L O T _ A ] ) ,   " " ) ;  
 	 	 i n i M e m c a r d S e c t i o n - > G e t ( " D e f a u l t M e m c a r d B " ,   & ( D e f a u l t M e m c a r d [ S L O T _ B ] ) ,   " " ) ;  
 	 	 i n i M e m c a r d S e c t i o n - > G e t ( " D e f a u l t I O F o l d e r " ,   & D e f a u l t I O P a t h ,   " / U s e r s / G C " ) ;  
  
 	 	 i n i M e m c a r d S e c t i o n - > G e t ( " U s e   P a g e s " ,   & m c m S e t t i n g s . u s e P a g e s ,   t r u e ) ;  
 	 	 i n i M e m c a r d S e c t i o n - > G e t ( " c B a n n e r " ,   & m c m S e t t i n g s . c o l u m n [ C O L U M N _ B A N N E R ] ,   t r u e ) ;  
 	 	 i n i M e m c a r d S e c t i o n - > G e t ( " c T i t l e " ,   & m c m S e t t i n g s . c o l u m n [ C O L U M N _ T I T L E ] ,   t r u e ) ;  
 	 	 i n i M e m c a r d S e c t i o n - > G e t ( " c C o m m e n t " ,   & m c m S e t t i n g s . c o l u m n [ C O L U M N _ C O M M E N T ] ,   t r u e ) ;  
 	 	 i n i M e m c a r d S e c t i o n - > G e t ( " c I c o n " ,   & m c m S e t t i n g s . c o l u m n [ C O L U M N _ I C O N ] ,   t r u e ) ;  
 	 	 i n i M e m c a r d S e c t i o n - > G e t ( " c B l o c k s " ,   & m c m S e t t i n g s . c o l u m n [ C O L U M N _ B L O C K S ] ,   t r u e ) ;  
 	 	 i n i M e m c a r d S e c t i o n - > G e t ( " c F i r s t   B l o c k " ,   & m c m S e t t i n g s . c o l u m n [ C O L U M N _ F I R S T B L O C K ] ,   t r u e ) ;  
  
 	 	 m c m S e t t i n g s . c o l u m n [ N U M B E R _ O F _ C O L U M N ]   =   f a l s e ;  
  
 	 	 f o r ( i n t   i   =   C O L U M N _ G A M E C O D E ;   i   <   N U M B E R _ O F _ C O L U M N ;   i + + )  
 	 	 {  
 	 	 	 m c m S e t t i n g s . c o l u m n [ i ]   =   m c m S e t t i n g s . c o l u m n [ N U M B E R _ O F _ C O L U M N ] ;  
 	 	 }  
 	 	 r e t u r n   t r u e ;  
 	 }  
 	 r e t u r n   f a l s e ;  
 }  
  
 b o o l   C M e m c a r d M a n a g e r : : S a v e S e t t i n g s ( )  
 {  
 	 M e m c a r d M a n a g e r I n i . L o a d ( F i l e : : G e t U s e r P a t h ( F _ D O L P H I N C O N F I G _ I D X ) ) ;  
 	 i n i M e m c a r d S e c t i o n   =   M e m c a r d M a n a g e r I n i . G e t O r C r e a t e S e c t i o n ( " M e m c a r d M a n a g e r " ) ;  
 	 i n i M e m c a r d S e c t i o n - > S e t ( " I t e m s   p e r   p a g e " ,     i t e m s P e r P a g e ,   1 6 ) ;  
 	 i n i M e m c a r d S e c t i o n - > S e t ( " D e f a u l t M e m c a r d A " ,   D e f a u l t M e m c a r d [ S L O T _ A ] ,   " " ) ;  
 	 i n i M e m c a r d S e c t i o n - > S e t ( " D e f a u l t M e m c a r d B " ,   D e f a u l t M e m c a r d [ S L O T _ B ] ,   " " ) ;  
  
 	 i n i M e m c a r d S e c t i o n - > S e t ( " U s e   P a g e s " ,   m c m S e t t i n g s . u s e P a g e s ,   t r u e ) ;  
 	 i n i M e m c a r d S e c t i o n - > S e t ( " c B a n n e r " ,   m c m S e t t i n g s . c o l u m n [ C O L U M N _ B A N N E R ] ,   t r u e ) ;  
 	 i n i M e m c a r d S e c t i o n - > S e t ( " c T i t l e " ,   m c m S e t t i n g s . c o l u m n [ C O L U M N _ T I T L E ] ,   t r u e ) ;  
 	 i n i M e m c a r d S e c t i o n - > S e t ( " c C o m m e n t " ,   m c m S e t t i n g s . c o l u m n [ C O L U M N _ C O M M E N T ] ,   t r u e ) ;  
 	 i n i M e m c a r d S e c t i o n - > S e t ( " c I c o n " , 	 m c m S e t t i n g s . c o l u m n [ C O L U M N _ I C O N ] ,   t r u e ) ;  
 	 i n i M e m c a r d S e c t i o n - > S e t ( " c B l o c k s " ,   m c m S e t t i n g s . c o l u m n [ C O L U M N _ B L O C K S ] ,   t r u e ) ;  
 	 i n i M e m c a r d S e c t i o n - > S e t ( " c F i r s t   B l o c k " ,   m c m S e t t i n g s . c o l u m n [ C O L U M N _ F I R S T B L O C K ] ,   t r u e ) ;  
  
 	 r e t u r n   M e m c a r d M a n a g e r I n i . S a v e ( F i l e : : G e t U s e r P a t h ( F _ D O L P H I N C O N F I G _ I D X ) ) ;  
 }  
  
 v o i d   C M e m c a r d M a n a g e r : : C r e a t e G U I C o n t r o l s ( )  
 {  
 	 / /   C r e a t e   t h e   c o n t r o l s   f o r   b o t h   m e m c a r d s  
  
 	 c o n s t   w x C h a r *   A R R O W [ 2 ]   =   { _ T ( " < - " ) ,   _ T ( " - > " ) } ;  
  
 	 m _ C o n v e r t T o G c i   =   n e w   w x B u t t o n ( t h i s ,   I D _ C O N V E R T T O G C I ,   w x T ( " C o n v e r t   t o   G C I " ) ) ;  
 	  
 	 f o r   ( i n t   s l o t   =   S L O T _ A ;   s l o t   <   S L O T _ B   +   1 ;   s l o t + + )  
 	 {  
 	 	 m _ C o p y F r o m [ s l o t ] 	 =   n e w   w x B u t t o n ( t h i s ,   I D _ C O P Y F R O M _ A   +   s l o t ,  
 	 	 	 w x S t r i n g : : F o r m a t ( w x T ( " % 1 $ s C o p y % 1 $ s " ) ,   A R R O W [ s l o t   ?   0   :   1 ] ) ) ;  
 	 	 m _ S a v e I m p o r t [ s l o t ] 	 =   n e w   w x B u t t o n ( t h i s ,   I D _ S A V E I M P O R T _ A   +   s l o t ,  
 	 	 	 w x S t r i n g : : F o r m a t ( w x T ( " % s I m p o r t   G C I % s " ) ,   A R R O W S ) ) ;  
 	 	 m _ S a v e E x p o r t [ s l o t ] 	 =   n e w   w x B u t t o n ( t h i s ,   I D _ S A V E E X P O R T _ A   +   s l o t ,  
 	 	 	 w x S t r i n g : : F o r m a t ( w x T ( " % s E x p o r t   G C I % s " ) ,   A R R O W S ) ) ;  
 	 	 m _ D e l e t e [ s l o t ] 	 	 =   n e w   w x B u t t o n ( t h i s ,   I D _ D E L E T E _ A   +   s l o t ,  
 	 	 	 w x S t r i n g : : F o r m a t ( w x T ( " % s D e l e t e % s " ) ,   A R R O W S ) ) ;  
  
  
 	 	 m _ P r e v P a g e [ s l o t ]   =   n e w   w x B u t t o n ( t h i s ,   I D _ P R E V P A G E _ A   +   s l o t ,   w x T ( " 
N Nu�" ) ) ;  
 	 	 m _ N e x t P a g e [ s l o t ]   =   n e w   w x B u t t o n ( t h i s ,   I D _ N E X T P A G E _ A   +   s l o t ,   w x T ( " N Nu�" ) ) ;  
  
 	 	 t _ S t a t u s [ s l o t ]   =   n e w   w x S t a t i c T e x t ( t h i s ,   0 ,   w x E m p t y S t r i n g ,   w x D e f a u l t P o s i t i o n , w x D e f a u l t S i z e ,   0 ,   w x E m p t y S t r i n g ) ;  
  
 	 	 s P a g e s [ s l o t ]   =   n e w   w x B o x S i z e r ( w x H O R I Z O N T A L ) ;  
 	 	 s P a g e s [ s l o t ] - > A d d ( m _ P r e v P a g e [ s l o t ] ,   0 ,   w x E X P A N D | w x A L L ,   1 ) ;  
 	 	 s P a g e s [ s l o t ] - > A d d ( t _ S t a t u s [ s l o t ] , 0 ,   w x E X P A N D | w x A L L ,   5 ) ;  
 	 	 s P a g e s [ s l o t ] - > A d d ( 0 ,   0 ,   1 ,   w x E X P A N D | w x A L L ,   0 ) ;  
 	 	 s P a g e s [ s l o t ] - > A d d ( m _ N e x t P a g e [ s l o t ] ,   0 ,   w x E X P A N D | w x A L L ,   1 ) ;  
  
 	 	 m _ M e m c a r d P a t h [ s l o t ]   =   n e w   w x F i l e P i c k e r C t r l ( t h i s ,   I D _ M E M C A R D P A T H _ A   +   s l o t ,  
 	 	 	   w x S t r i n g : : F r o m 8 B i t D a t a ( F i l e : : G e t U s e r P a t h ( D _ G C U S E R _ I D X ) ) ,   w x T ( " 	��b�QX[aS: " ) ,  
 	 	 w x T ( " G a m e c u b e   �QX[aS  ( * . r a w , * . g c p ) | * . r a w ; * . g c p " ) ,   w x D e f a u l t P o s i t i o n ,   w x D e f a u l t S i z e ,   w x F L P _ U S E _ T E X T C T R L | w x F L P _ O P E N ) ;  
 	  
 	 	 m _ M e m c a r d L i s t [ s l o t ]   =   n e w   C M e m c a r d L i s t C t r l ( t h i s ,   I D _ M E M C A R D L I S T _ A   +   s l o t ,   w x D e f a u l t P o s i t i o n ,   w x S i z e ( 3 5 0 , 4 0 0 ) ,  
 	 	 w x L C _ R E P O R T   |   w x S U N K E N _ B O R D E R   |   w x L C _ A L I G N _ L E F T   |   w x L C _ S I N G L E _ S E L ,   m c m S e t t i n g s ) ;  
 	  
 	 	 m _ M e m c a r d L i s t [ s l o t ] - > A s s i g n I m a g e L i s t ( n e w   w x I m a g e L i s t ( 9 6 , 3 2 ) , w x I M A G E _ L I S T _ S M A L L ) ;  
  
 	 	 s M e m c a r d [ s l o t ]   =   n e w   w x S t a t i c B o x S i z e r ( w x V E R T I C A L ,   t h i s ,   w x S t r i n g : : F o r m a t ( w x T ( " �QX[aS  % c " ) ,   ' A '   +   s l o t ) ) ;  
 	 	 s M e m c a r d [ s l o t ] - > A d d ( m _ M e m c a r d P a t h [ s l o t ] ,   0 ,   w x E X P A N D | w x A L L ,   5 ) ;  
 	 	 s M e m c a r d [ s l o t ] - > A d d ( m _ M e m c a r d L i s t [ s l o t ] ,   1 ,   w x E X P A N D | w x A L L ,   5 ) ;  
 	 	 s M e m c a r d [ s l o t ] - > A d d ( s P a g e s [ s l o t ] ,   0 ,   w x E X P A N D | w x A L L ,   1 ) ;  
 	 }  
  
 	 s B u t t o n s   =   n e w   w x B o x S i z e r ( w x V E R T I C A L ) ;  
 	 s B u t t o n s - > A d d S t r e t c h S p a c e r ( 2 ) ;  
 	 s B u t t o n s - > A d d ( m _ C o p y F r o m [ S L O T _ B ] ,   0 ,   w x E X P A N D ,   5 ) ;  
 	 s B u t t o n s - > A d d ( m _ C o p y F r o m [ S L O T _ A ] ,   0 ,   w x E X P A N D ,   5 ) ;  
 	 s B u t t o n s - > A d d S t r e t c h S p a c e r ( 1 ) ;  
 	 s B u t t o n s - > A d d ( m _ S a v e I m p o r t [ S L O T _ A ] ,   0 ,   w x E X P A N D ,   5 ) ;  
 	 s B u t t o n s - > A d d ( m _ S a v e E x p o r t [ S L O T _ A ] ,   0 ,   w x E X P A N D ,   5 ) ;  
 	 s B u t t o n s - > A d d S t r e t c h S p a c e r ( 1 ) ;  
 	 s B u t t o n s - > A d d ( m _ C o n v e r t T o G c i ,   0 ,   w x E X P A N D ,   5 ) ;  
 	 s B u t t o n s - > A d d S t r e t c h S p a c e r ( 1 ) ;  
 	 s B u t t o n s - > A d d ( m _ S a v e I m p o r t [ S L O T _ B ] ,   0 ,   w x E X P A N D ,   5 ) ;  
 	 s B u t t o n s - > A d d ( m _ S a v e E x p o r t [ S L O T _ B ] ,   0 ,   w x E X P A N D ,   5 ) ;  
 	 s B u t t o n s - > A d d S t r e t c h S p a c e r ( 1 ) ;  
 	 s B u t t o n s - > A d d ( m _ D e l e t e [ S L O T _ A ] ,   0 ,   w x E X P A N D ,   5 ) ;  
 	 s B u t t o n s - > A d d ( m _ D e l e t e [ S L O T _ B ] ,   0 ,   w x E X P A N D ,   5 ) ;  
 	 s B u t t o n s - > A d d S t r e t c h S p a c e r ( 1 ) ;  
  
 	 s M a i n   =   n e w   w x B o x S i z e r ( w x H O R I Z O N T A L ) ;  
 	 s M a i n - > A d d ( s M e m c a r d [ S L O T _ A ] ,   1 ,   w x E X P A N D | w x A L L ,   5 ) ;  
 	 s M a i n - > A d d ( s B u t t o n s ,   0 ,   w x E X P A N D ,   0 ) ;  
 	 s M a i n - > A d d ( s M e m c a r d [ S L O T _ B ] ,   1 ,   w x E X P A N D | w x A L L ,   5 ) ;  
  
 	 t h i s - > S e t S i z e r ( s M a i n ) ;  
 	 s M a i n - > S e t S i z e H i n t s ( t h i s ) ;  
 	 F i t ( ) ;  
 	 C e n t e r ( ) ;  
  
 	 f o r   ( i n t   i   =   S L O T _ A ;   i   <   S L O T _ B   +   1 ;   i + + )  
 	 {  
 	 	 m _ P r e v P a g e [ i ] - > D i s a b l e ( ) ;  
 	 	 m _ N e x t P a g e [ i ] - > D i s a b l e ( ) ;  
 	 	 m _ C o p y F r o m [ i ] - > D i s a b l e ( ) ;  
 	 	 m _ S a v e I m p o r t [ i ] - > D i s a b l e ( ) ;  
 	 	 m _ S a v e E x p o r t [ i ] - > D i s a b l e ( ) ;  
 	 	 m _ D e l e t e [ i ] - > D i s a b l e ( ) ;  
 	 	 i f   ( D e f a u l t M e m c a r d [ i ] . l e n g t h ( ) )  
 	 	 {  
 	 	 	 m _ M e m c a r d P a t h [ i ] - > S e t P a t h ( w x S t r i n g : : F r o m 8 B i t D a t a ( D e f a u l t M e m c a r d [ i ] . c _ s t r ( ) ) ) ;  
 	 	 	 C h a n g e P a t h ( i ) ;  
 	 	 }  
 	 }  
 }  
  
 v o i d   C M e m c a r d M a n a g e r : : O n C l o s e ( w x C l o s e E v e n t &   W X U N U S E D   ( e v e n t ) )  
 {  
 	 D e s t r o y ( ) ;  
 }  
  
 v o i d   C M e m c a r d M a n a g e r : : O n P a t h C h a n g e ( w x F i l e D i r P i c k e r E v e n t &   e v e n t )  
 {  
 	 C h a n g e P a t h ( e v e n t . G e t I d ( )   -   I D _ M E M C A R D P A T H _ A ) ;  
 }  
  
 v o i d   C M e m c a r d M a n a g e r : : C h a n g e P a t h ( i n t   s l o t )  
 {  
 	 i n t   s l o t 2   =   ( s l o t   = =   S L O T _ A )   ?   S L O T _ B   :   S L O T _ A ;  
 	 p a g e [ s l o t ]   =   F I R S T P A G E ;  
 	 i f   ( m c m S e t t i n g s . u s e P a g e s   & &   m _ P r e v P a g e [ s l o t ] - > I s E n a b l e d ( ) )  
 	 {  
 	 	 m _ P r e v P a g e [ s l o t ] - > D i s a b l e ( ) ;  
 	 	 m _ M e m c a r d L i s t [ s l o t ] - > p r e v P a g e   =   f a l s e ;  
 	 }  
 	 i f   ( ! s t r c a s e c m p ( m _ M e m c a r d P a t h [ s l o t 2 ] - > G e t P a t h ( ) . m b _ s t r ( ) ,   m _ M e m c a r d P a t h [ s l o t ] - > G e t P a t h ( ) . m b _ s t r ( ) ) )  
 	 {  
 	 	 i f ( m _ M e m c a r d P a t h [ s l o t ] - > G e t P a t h ( ) . l e n g t h ( ) )  
 	 	 	 P a n i c A l e r t ( " M e m c a r d   a l r e a d y   o p e n e d " ) ;  
 	 }  
 	 e l s e  
 	 {  
 	 	 i f   ( m _ M e m c a r d P a t h [ s l o t ] - > G e t P a t h ( ) . l e n g t h ( )   & &   R e l o a d M e m c a r d ( m _ M e m c a r d P a t h [ s l o t ] - > G e t P a t h ( ) . m b _ s t r ( ) ,   s l o t ) )  
 	 	 {  
 	 	 	 m c m S e t t i n g s . t w o C a r d s L o a d e d   =   t r u e ;  
 	 	 	 m _ S a v e I m p o r t [ s l o t ] - > E n a b l e ( ) ;  
 	 	 	 m _ S a v e E x p o r t [ s l o t ] - > E n a b l e ( ) ;  
 	 	 	 m _ D e l e t e [ s l o t ] - > E n a b l e ( ) ;  
 	 	 }  
 	 	 e l s e  
 	 	 {  
 	 	 	 i f   ( m e m o r y C a r d [ s l o t ] )  
 	 	 	 {  
 	 	 	 	 d e l e t e   m e m o r y C a r d [ s l o t ] ;  
 	 	 	 	 m e m o r y C a r d [ s l o t ]   =   N U L L ;  
 	 	 	 }  
 	 	 	 m c m S e t t i n g s . t w o C a r d s L o a d e d   =   f a l s e ;  
 	 	 	 m _ M e m c a r d P a t h [ s l o t ] - > S e t P a t h ( w x E m p t y S t r i n g ) ;  
 	 	 	 m _ M e m c a r d L i s t [ s l o t ] - > C l e a r A l l ( ) ;  
 	 	 	 t _ S t a t u s [ s l o t ] - > S e t L a b e l ( w x E m p t y S t r i n g ) ;  
 	 	 	 m _ S a v e I m p o r t [ s l o t ] - > D i s a b l e ( ) ;  
 	 	 	 m _ S a v e E x p o r t [ s l o t ] - > D i s a b l e ( ) ;  
 	 	 	 m _ D e l e t e [ s l o t ] - > D i s a b l e ( ) ;  
 	 	 	 i f   ( m c m S e t t i n g s . u s e P a g e s )  
 	 	 	 {  
 	 	 	 	 m _ P r e v P a g e [ s l o t ] - > D i s a b l e ( ) ;  
 	 	 	 	 m _ N e x t P a g e [ s l o t ] - > D i s a b l e ( ) ;  
 	 	 	 }  
 	 	 }  
 	 }  
 	 i f   ( m _ D e l e t e [ S L O T _ A ] - > I s E n a b l e d ( )   & &   m _ D e l e t e [ S L O T _ B ] - > I s E n a b l e d ( ) )  
 	 {  
 	 	 m _ C o p y F r o m [ S L O T _ A ] - > E n a b l e ( ) ;  
 	 	 m _ C o p y F r o m [ S L O T _ B ] - > E n a b l e ( ) ;  
 	 }  
 	 e l s e  
 	 {  
 	 	 m _ C o p y F r o m [ S L O T _ A ] - > D i s a b l e ( ) ;  
 	 	 m _ C o p y F r o m [ S L O T _ B ] - > D i s a b l e ( ) ;  
 	 }  
 }  
  
 v o i d   C M e m c a r d M a n a g e r : : O n P a g e C h a n g e ( w x C o m m a n d E v e n t &   e v e n t )  
 {  
 	 i n t   s l o t   =   S L O T _ B ;  
 	 s w i t c h   ( e v e n t . G e t I d ( ) )  
 	 {  
 	 c a s e   I D _ N E X T P A G E _ A :  
 	 	 s l o t   =   S L O T _ A ;  
 	 c a s e   I D _ N E X T P A G E _ B :  
 	 	 i f   ( ! m _ P r e v P a g e [ s l o t ] - > I s E n a b l e d ( ) )  
 	 	 {  
 	 	 	 m _ P r e v P a g e [ s l o t ] - > E n a b l e ( ) ;  
 	 	 	 m _ M e m c a r d L i s t [ s l o t ] - > p r e v P a g e   =   t r u e ;  
 	 	 }  
 	 	 p a g e [ s l o t ] + + ;  
 	 	 i f   ( p a g e [ s l o t ]   = =   m a x P a g e s )  
 	 	 {  
 	 	 	 m _ N e x t P a g e [ s l o t ] - > D i s a b l e ( ) ;  
 	 	 	 m _ M e m c a r d L i s t [ s l o t ] - > n e x t P a g e   =   f a l s e ;  
 	 	 }  
 	 	 R e l o a d M e m c a r d ( m _ M e m c a r d P a t h [ s l o t ] - > G e t P a t h ( ) . m b _ s t r ( ) ,   s l o t ) ;  
 	 	 b r e a k ;  
 	 c a s e   I D _ P R E V P A G E _ A :  
 	 	 s l o t   =   S L O T _ A ;  
 	 c a s e   I D _ P R E V P A G E _ B :  
 	 	 i f   ( ! m _ N e x t P a g e [ s l o t ] - > I s E n a b l e d ( ) )  
 	 	 {  
 	 	 	 m _ N e x t P a g e [ s l o t ] - > E n a b l e ( ) ;  
 	 	 	 m _ M e m c a r d L i s t [ s l o t ] - > n e x t P a g e   =   t r u e ;  
 	 	 }  
 	 	 p a g e [ s l o t ] - - ;  
 	 	 i f   ( ! p a g e [ s l o t ] )  
 	 	 {  
 	 	 	 m _ P r e v P a g e [ s l o t ] - > D i s a b l e ( ) ;  
 	 	 	 m _ M e m c a r d L i s t [ s l o t ] - > p r e v P a g e   =   f a l s e ;  
 	 	 }  
 	 	 R e l o a d M e m c a r d ( m _ M e m c a r d P a t h [ s l o t ] - > G e t P a t h ( ) . m b _ s t r ( ) ,   s l o t ) ;  
 	 	 b r e a k ;  
 	 }  
 }  
  
 v o i d   C M e m c a r d M a n a g e r : : O n M e n u C h a n g e ( w x C o m m a n d E v e n t &   e v e n t )  
 {  
 	 i n t   _ i d   =   e v e n t . G e t I d ( ) ;  
 	 s w i t c h ( _ i d )  
 	 {  
 	 c a s e   I D _ M E M C A R D P A T H _ A :  
 	 c a s e   I D _ M E M C A R D P A T H _ B :  
 	 	 D e f a u l t M e m c a r d [ _ i d   -   I D _ M E M C A R D P A T H _ A ]   =   m _ M e m c a r d P a t h [ _ i d   -   I D _ M E M C A R D P A T H _ A ] - > G e t P a t h ( ) . m b _ s t r ( ) ;  
 	 	 r e t u r n ;  
 	 c a s e   I D _ U S E P A G E S :  
 	 	 m c m S e t t i n g s . u s e P a g e s   =   ! m c m S e t t i n g s . u s e P a g e s ;  
 	 	 i f   ( ! m c m S e t t i n g s . u s e P a g e s )  
 	 	 {  
 	 	 	 m _ P r e v P a g e [ S L O T _ A ] - > D i s a b l e ( ) ;  
 	 	 	 m _ P r e v P a g e [ S L O T _ B ] - > D i s a b l e ( ) ;  
 	 	 	 m _ N e x t P a g e [ S L O T _ A ] - > D i s a b l e ( ) ;  
 	 	 	 m _ N e x t P a g e [ S L O T _ B ] - > D i s a b l e ( ) ;  
 	 	 	 m _ M e m c a r d L i s t [ S L O T _ A ] - > p r e v P a g e   =  
 	 	 	 m _ M e m c a r d L i s t [ S L O T _ B ] - > p r e v P a g e   =   f a l s e ;  
 	 	 	 p a g e [ S L O T _ A ]   =  
 	 	 	 p a g e [ S L O T _ B ]   =   F I R S T P A G E ;  
 	 	 }  
 	 	 b r e a k ;  
 	 c a s e   N U M B E R _ O F _ C O L U M N :  
 	 	 f o r   ( i n t   i   =   C O L U M N _ G A M E C O D E ;   i   < =   N U M B E R _ O F _ C O L U M N ;   i + + )  
 	 	 {  
 	 	 	 m c m S e t t i n g s . c o l u m n [ i ]   =   ! m c m S e t t i n g s . c o l u m n [ i ] ;  
 	 	 }  
 	 	 b r e a k ;  
 	 d e f a u l t :  
 	 	 m c m S e t t i n g s . c o l u m n [ _ i d ]   =   ! m c m S e t t i n g s . c o l u m n [ _ i d ] ;  
 	 	 b r e a k ;  
 	 }  
  
 	 i f   ( m e m o r y C a r d [ S L O T _ A ] ) 	 R e l o a d M e m c a r d ( m _ M e m c a r d P a t h [ S L O T _ A ] - > G e t P a t h ( ) . m b _ s t r ( ) ,   S L O T _ A ) ;  
 	 i f   ( m e m o r y C a r d [ S L O T _ B ] ) 	 R e l o a d M e m c a r d ( m _ M e m c a r d P a t h [ S L O T _ B ] - > G e t P a t h ( ) . m b _ s t r ( ) ,   S L O T _ B ) ;  
 }  
 b o o l   C M e m c a r d M a n a g e r : : C o p y D e l e t e S w i t c h ( u 3 2   e r r o r ,   i n t   s l o t )  
 {  
 	 s w i t c h   ( e r r o r )  
 	 {  
 	 c a s e   G C S :  
 	 	 S u c c e s s A l e r t ( " F i l e   c o n v e r t e d   t o   . g c i " ) ;  
 	 	 b r e a k ;  
 	 c a s e   S U C C E S S :  
 	 	 i f   ( s l o t   ! =   - 1 )  
 	 	 {  
 	 	 	 m e m o r y C a r d [ s l o t ] - > F i x C h e c k s u m s ( ) ;  
 	 	 	 i f   ( ! m e m o r y C a r d [ s l o t ] - > S a v e ( ) )   P a n i c A l e r t ( E _ S A V E F A I L E D ) ;  
 	 	 	 p a g e [ s l o t ]   =   F I R S T P A G E ;  
 	 	 	 R e l o a d M e m c a r d ( m _ M e m c a r d P a t h [ s l o t ] - > G e t P a t h ( ) . m b _ s t r ( ) ,   s l o t ) ;  
 	 	 }  
 	 	 b r e a k ;  
 	 c a s e   N O M E M C A R D :  
 	 	 P a n i c A l e r t ( " F i l e   i s   n o t   r e c o g n i z e d   a s   a   m e m c a r d " ) ;  
 	 	 b r e a k ;  
 	 c a s e   O P E N F A I L :  
 	 	 P a n i c A l e r t ( " F i l e   c o u l d   n o t   b e   o p e n e d \ n o r   d o e s   n o t   h a v e   a   v a l i d   e x t e n s i o n " ) ;  
 	 	 b r e a k ;  
 	 c a s e   O U T O F B L O C K S :  
 	 	 i f   ( s l o t   = =   - 1 )  
 	 	 {  
 	 	 	 P a n i c A l e r t ( E _ U N K ) ;  
 	 	 	 b r e a k ;  
 	 	 }  
 	 	 P a n i c A l e r t ( " O n l y   % d   b l o c k s   a v a i l a b l e " ,   m e m o r y C a r d [ s l o t ] - > G e t F r e e B l o c k s ( ) ) ;  
 	 	 b r e a k ;  
 	 c a s e   O U T O F D I R E N T R I E S :  
 	 	 P a n i c A l e r t ( " N o   f r e e   d i r   i n d e x   e n t r i e s " ) ;  
 	 	 b r e a k ;  
 	 c a s e   L E N G T H F A I L :  
 	 	 P a n i c A l e r t ( " I m p o r t e d   f i l e   h a s   i n v a l i d   l e n g t h " ) ;  
 	 	 b r e a k ;  
 	 c a s e   I N V A L I D F I L E S I Z E :  
 	 	 P a n i c A l e r t ( " T h e   s a v e   y o u   a r e   t r y i n g   t o   c o p y   h a s   a n   i n v a l i d   f i l e   s i z e " ) ;  
 	 	 b r e a k ;  
 	 c a s e   T I T L E P R E S E N T :  
 	 	 P a n i c A l e r t ( " M e m c a r d   a l r e a d y   h a s   a   s a v e   f o r   t h i s   t i t l e " ) ;  
 	 	 b r e a k ;  
 	 c a s e   S A V F A I L :  
 	 	 P a n i c A l e r t ( " I m p o r t e d   f i l e   h a s   s a v   e x t e n s i o n \ n b u t   d o e s   n o t   h a v e   a   c o r r e c t   h e a d e r " ) ;  
 	 	 b r e a k ;  
 	 c a s e   G C S F A I L :  
 	 	 P a n i c A l e r t ( " I m p o r t e d   f i l e   h a s   g s c   e x t e n s i o n \ n b u t   d o e s   n o t   h a v e   a   c o r r e c t   h e a d e r " ) ;  
 	 	 b r e a k ;  
 	 c a s e   F A I L :  
 	 	 i f   ( s l o t   = =   - 1 )  
 	 	 {  
 	 	 	 P a n i c A l e r t ( " E x p o r t   F a i l e d " ) ;  
 	 	 	 r e t u r n   f a l s e ;  
 	 	 }  
 	 	 P a n i c A l e r t ( " I n v a l i d   b a t . m a p   o r   d i r   e n t r y " ) ;  
 	 	 b r e a k ;  
 	 c a s e   W R I T E F A I L :  
 	 	 P a n i c A l e r t ( E _ S A V E F A I L E D ) ;  
 	 	 b r e a k ;  
 	 c a s e   D E L E T E _ F A I L :  
 	 	 P a n i c A l e r t ( " O r d e r   o f   f i l e s   i n   t h e   F i l e   D i r e c t o r y   d o   n o t   m a t c h   t h e   b l o c k   o r d e r \ n "  
 	 	 	 	 " R i g h t   c l i c k   a n d   e x p o r t   a l l   o f   t h e   s a v e s , \ n a n d   i m p o r t   t h e   t h e   s a v e s   t o   a   n e w   m e m c a r d \ n " ) ;  
 	 	 b r e a k ;  
 	 d e f a u l t :  
 	 	 P a n i c A l e r t ( E _ U N K ) ;  
 	 	 b r e a k ;  
 	 }  
 	 r e t u r n   t r u e ;  
 }  
  
 v o i d   C M e m c a r d M a n a g e r : : C o p y D e l e t e C l i c k ( w x C o m m a n d E v e n t &   e v e n t )  
 {  
 	  
 	 i n t   i n d e x _ A   =   m _ M e m c a r d L i s t [ S L O T _ A ] - > G e t N e x t I t e m ( - 1 ,   w x L I S T _ N E X T _ A L L ,   w x L I S T _ S T A T E _ S E L E C T E D ) ;  
 	 i n t   i n d e x _ B   =   m _ M e m c a r d L i s t [ S L O T _ B ] - > G e t N e x t I t e m ( - 1 ,   w x L I S T _ N E X T _ A L L ,   w x L I S T _ S T A T E _ S E L E C T E D ) ;  
 	 i n t   s l o t   =   S L O T _ B ;  
 	 i n t   s l o t 2   =   S L O T _ A ;  
 	 i n t   i n d e x   =   i n d e x _ B ;  
 	 s t d : : s t r i n g   f i l e N a m e 2 ( " " ) ;  
  
 	 i f   ( i n d e x _ A   ! =   w x N O T _ F O U N D   & &   p a g e [ S L O T _ A ] )   i n d e x _ A   + =   i t e m s P e r P a g e   *   p a g e [ S L O T _ A ] ;  
 	 i f   ( i n d e x _ B   ! =   w x N O T _ F O U N D   & &   p a g e [ S L O T _ B ] )   i n d e x _ B   + =   i t e m s P e r P a g e   *   p a g e [ S L O T _ B ] ;  
  
 	 s w i t c h   ( e v e n t . G e t I d ( ) )  
 	 {  
 	 c a s e   I D _ C O P Y F R O M _ B :  
 	 	 s l o t   =   S L O T _ A ;  
 	 	 s l o t 2   =   S L O T _ B ;  
 	 c a s e   I D _ C O P Y F R O M _ A :  
 	 	 i n d e x   =   s l o t 2   ?   i n d e x _ B   :   i n d e x _ A ;  
 	 	 i f   ( ( i n d e x   ! =   w x N O T _ F O U N D ) )  
 	 	 {  
 	 	 	 C o p y D e l e t e S w i t c h ( m e m o r y C a r d [ s l o t ] - > C o p y F r o m ( * m e m o r y C a r d [ s l o t 2 ] ,   i n d e x ) ,   s l o t ) ;  
 	 	 }  
 	 	 b r e a k ;  
 	 c a s e   I D _ F I X C H E C K S U M _ A :  
 	 	 s l o t   =   S L O T _ A ;  
 	 c a s e   I D _ F I X C H E C K S U M _ B :  
 	 	 i f   ( m e m o r y C a r d [ s l o t ] - > F i x C h e c k s u m s ( )   & &   m e m o r y C a r d [ s l o t ] - > S a v e ( ) )  
 	 	 {  
 	 	 	 S u c c e s s A l e r t ( " T h e   c h e c k s u m   w a s   s u c c e s s f u l l y   f i x e d " ) ;  
 	 	 }  
 	 	 e l s e   P a n i c A l e r t ( E _ S A V E F A I L E D ) ;  
 	 	 b r e a k ;    
 	 c a s e   I D _ C O N V E R T T O G C I :  
 	 	 f i l e N a m e 2   =   " c o n v e r t " ;  
 	 c a s e   I D _ S A V E I M P O R T _ A :  
 	 	 s l o t   =   S L O T _ A ;  
 	 c a s e   I D _ S A V E I M P O R T _ B :  
 	 {  
 	 	 w x S t r i n g   f i l e N a m e   =   w x F i l e S e l e c t o r (  
 	 	 	 w x T ( " 	��b ����[eQ�vX[ch�e�N" ) ,  
 	 	 	 ( s t r c m p ( D e f a u l t I O P a t h . c _ s t r ( ) ,   " / U s e r s / G C " )   = =   0 )  
 	 	 	 	 ?   w x S t r i n g : : F r o m A s c i i ( " " )  
 	 	 	 	 :   w x S t r i n g : : F r o m 8 B i t D a t a ( D e f a u l t I O P a t h . c _ s t r ( ) ) ,  
 	 	 	 w x E m p t y S t r i n g ,   w x E m p t y S t r i n g ,  
 	 	 	 w x T ( " N a t i v e   G C I   f i l e s ( * . g c i ) | * . g c i | " )  
 	 	 	 w x T ( " M a d C a t z   G a m e s h a r k   f i l e s ( * . g c s ) | * . g c s | " )  
 	 	 	 w x T ( " D a t e l   M a x D r i v e / P r o   f i l e s ( * . s a v ) | * . s a v " ) ,  
 	 	 	 w x F D _ O P E N   |   w x F D _ F I L E _ M U S T _ E X I S T ) ;  
 	 	 i f   ( ! f i l e N a m e . e m p t y ( )   & &   ! f i l e N a m e 2 . e m p t y ( ) )  
 	 	 {  
 	 	 	 w x S t r i n g   t e m p 2   =   w x F i l e S e l e c t o r ( w x T ( " �OX[  G C I   :N. . " ) ,  
 	 	 	 	 w x E m p t y S t r i n g ,   w x E m p t y S t r i n g ,   w x T ( " . g c i " ) ,  
 	 	 	 	 w x T ( " G C I   �e�N( * . g c i ) | * . g c i " ) ,  
 	 	 	 	 w x F D _ O V E R W R I T E _ P R O M P T | w x F D _ S A V E ) ;  
 	 	 	 i f   ( t e m p 2 . e m p t y ( ) )   b r e a k ;  
 	 	 	 f i l e N a m e 2   =   t e m p 2 . m b _ s t r ( ) ;  
 	 	 }  
 	 	 i f   ( f i l e N a m e . l e n g t h ( )   >   0 )  
 	 	 {  
 	 	 	 C o p y D e l e t e S w i t c h ( m e m o r y C a r d [ s l o t ] - > I m p o r t G c i ( f i l e N a m e . m b _ s t r ( ) ,   f i l e N a m e 2 ) ,   s l o t ) ;  
 	 	 }  
 	 }  
 	 b r e a k ;  
 	 c a s e   I D _ S A V E E X P O R T _ A :  
 	 	 s l o t = S L O T _ A ;  
 	 	 i n d e x   =   i n d e x _ A ;  
 	 c a s e   I D _ S A V E E X P O R T _ B :  
 	 	 i f   ( i n d e x   ! =   w x N O T _ F O U N D )  
 	 	 {  
 	 	 	 c h a r   t e m p C [ 1 0   +   D E N T R Y _ S T R L E N ] ,  
 	 	 	 	   t e m p C 2 [ D E N T R Y _ S T R L E N ] ;  
 	 	 	 m e m o r y C a r d [ s l o t ] - > D E n t r y _ G a m e C o d e ( i n d e x , t e m p C ) ;  
 	 	 	 m e m o r y C a r d [ s l o t ] - > D E n t r y _ F i l e N a m e ( i n d e x , t e m p C 2 ) ;  
 	 	 	 s p r i n t f ( t e m p C ,   " % s _ % s . g c i " ,   t e m p C ,   t e m p C 2 ) ;  
 	 	 	 w x S t r i n g   f i l e N a m e   =   w x F i l e S e l e c t o r (  
 	 	 	 	 w x T ( " �[�QX[ch:N. . " ) ,  
 	 	 	 	 w x S t r i n g : : F r o m 8 B i t D a t a ( D e f a u l t I O P a t h . c _ s t r ( ) ) ,  
 	 	 	 	 w x S t r i n g : : F r o m 8 B i t D a t a ( t e m p C ) ,   w x T ( " . g c i " ) ,  
 	 	 	 	 w x T ( " N a t i v e   G C I   f i l e s ( * . g c i ) | * . g c i | " )  
 	 	 	 	 w x T ( " M a d C a t z   G a m e s h a r k   f i l e s ( * . g c s ) | * . g c s | " )  
 	 	 	 	 w x T ( " D a t e l   M a x D r i v e / P r o   f i l e s ( * . s a v ) | * . s a v " ) ,  
 	 	 	 	 w x F D _ O V E R W R I T E _ P R O M P T | w x F D _ S A V E ) ;  
  
 	 	 	 i f   ( f i l e N a m e . l e n g t h ( )   >   0 )  
 	 	 	 {  
 	 	 	 	 i f   ( ! C o p y D e l e t e S w i t c h ( m e m o r y C a r d [ s l o t ] - > E x p o r t G c i ( i n d e x ,   f i l e N a m e . m b _ s t r ( ) ,   N U L L ) ,   - 1 ) )  
 	 	 	 	 {  
 	 	 	 	 	 F i l e : : D e l e t e ( f i l e N a m e . m b _ s t r ( ) ) ;  
 	 	 	 	 }  
 	 	 	 }  
 	 	 }  
 	 	 b r e a k ;  
 	 c a s e   I D _ E X P O R T A L L _ A :  
 	 	 s l o t = S L O T _ A ;  
 	 c a s e   I D _ E X P O R T A L L _ B :  
 	 {  
 	 	 s t d : : s t r i n g   p a t h 1 ,   p a t h 2 ,   m p a t h ;  
 	 	 m p a t h   =   m _ M e m c a r d P a t h [ s l o t ] - > G e t P a t h ( ) . m b _ s t r ( ) ;  
 	 	 S p l i t P a t h ( m p a t h ,   & p a t h 1 ,   & p a t h 2 ,   N U L L ) ;  
 	 	 p a t h 1   + =   p a t h 2 ;  
 	 	 F i l e : : C r e a t e D i r ( p a t h 1 . c _ s t r ( ) ) ;  
 	 	 i f ( P a n i c Y e s N o ( " W a r n i n g :   T h i s   w i l l   o v e r w r i t e   a n y   e x i s t i n g   s a v e s   "  
 	 	 	 	 	 " t h a t   a r e   i n   t h e   f o l d e r : \ n % s \ n a n d   h a v e   t h e   s a m e   n a m e "  
 	 	 	 	 	 "   a s   a   f i l e   o n   y o u r   m e m c a r d \ n C o n t i n u e ? " ,   p a t h 1 . c _ s t r ( ) ) )  
 	 	 f o r   ( i n t   i   =   0 ;   i   <   D I R L E N ;   i + + )  
 	 	 {  
 	 	 	 C o p y D e l e t e S w i t c h ( m e m o r y C a r d [ s l o t ] - > E x p o r t G c i ( i ,   " . " ,   & p a t h 1 ) ,   - 1 ) ;  
 	 	 }  
 	 	 b r e a k ;  
 	 }  
 	 c a s e   I D _ D E L E T E _ A :  
 	 	 s l o t   =   S L O T _ A ;  
 	 	 i n d e x   =   i n d e x _ A ;  
 	 c a s e   I D _ D E L E T E _ B :  
 	 	 i f   ( i n d e x   ! =   w x N O T _ F O U N D )  
 	 	 {  
 	 	 	 C o p y D e l e t e S w i t c h ( m e m o r y C a r d [ s l o t ] - > R e m o v e F i l e ( i n d e x ) ,   s l o t ) ;  
 	 	 }  
 	 	 b r e a k ;  
 	 }  
 }  
  
 b o o l   C M e m c a r d M a n a g e r : : R e l o a d M e m c a r d ( c o n s t   c h a r   * f i l e N a m e ,   i n t   c a r d )  
 {  
 	 i f   ( m e m o r y C a r d [ c a r d ] )   d e l e t e   m e m o r y C a r d [ c a r d ] ;  
  
 	 / /   T O D O :   a d d   e r r o r   c h e c k i n g   a n d   a n i m a t e   i c o n s  
 	 m e m o r y C a r d [ c a r d ]   =   n e w   G C M e m c a r d ( f i l e N a m e ) ;  
  
 	 i f   ( m e m o r y C a r d [ c a r d ] - > f a i l )   r e t u r n   f a l s e ;  
  
 	 i n t   j ;  
  
 	 w x S t r i n g   w x T i t l e ,  
 	 	 	   w x C o m m e n t ,  
 	 	 	   w x B l o c k ,  
 	 	 	   w x F i r s t B l o c k ,  
 	 	 	   w x L a b e l ,  
 	 	 	   t S t r i n g ;  
  
  
 	 m _ M e m c a r d L i s t [ c a r d ] - > H i d e ( ) ;  
 	 m _ M e m c a r d L i s t [ c a r d ] - > C l e a r A l l ( ) ;  
  
 	 m _ M e m c a r d L i s t [ c a r d ] - > I n s e r t C o l u m n ( C O L U M N _ B A N N E R ,   _ T ( " B a n n e r " ) ) ;  
 	 m _ M e m c a r d L i s t [ c a r d ] - > I n s e r t C o l u m n ( C O L U M N _ T I T L E ,   _ T ( " T i t l e " ) ) ;  
 	 m _ M e m c a r d L i s t [ c a r d ] - > I n s e r t C o l u m n ( C O L U M N _ C O M M E N T ,   _ T ( " C o m m e n t " ) ) ;  
 	 m _ M e m c a r d L i s t [ c a r d ] - > I n s e r t C o l u m n ( C O L U M N _ I C O N ,   _ T ( " I c o n " ) ) ;  
 	 m _ M e m c a r d L i s t [ c a r d ] - > I n s e r t C o l u m n ( C O L U M N _ B L O C K S ,   _ T ( " B l o c k s " ) ) ;  
 	 m _ M e m c a r d L i s t [ c a r d ] - > I n s e r t C o l u m n ( C O L U M N _ F I R S T B L O C K ,   _ T ( " F i r s t   B l o c k " ) ) ;  
  
 	 w x I m a g e L i s t   * l i s t   =   m _ M e m c a r d L i s t [ c a r d ] - > G e t I m a g e L i s t ( w x I M A G E _ L I S T _ S M A L L ) ;  
 	 l i s t - > R e m o v e A l l ( ) ;  
  
 	 i n t   n F i l e s   =   m e m o r y C a r d [ c a r d ] - > G e t N u m F i l e s ( ) ;  
 	 i n t   * i m a g e s   =   n e w   i n t [ n F i l e s * 2 ] ;  
  
 	 f o r   ( i n t   i   =   0 ; i   <   n F i l e s ; i + + )  
 	 {  
 	 	 s t a t i c   u 3 2   p x d a t a [ 9 6 * 3 2 ] ;  
 	 	 s t a t i c   u 8     a n i m D e l a y [ 8 ] ;  
 	 	 s t a t i c   u 3 2   a n i m D a t a [ 3 2 * 3 2 * 8 ] ;  
  
 	 	 i n t   n u m F r a m e s   =   m e m o r y C a r d [ c a r d ] - > R e a d A n i m R G B A 8 ( i , a n i m D a t a , a n i m D e l a y ) ;  
  
 	 	 i f   ( ! m e m o r y C a r d [ c a r d ] - > R e a d B a n n e r R G B A 8 ( i , p x d a t a ) )  
 	 	 {  
 	 	 	 m e m s e t ( p x d a t a , 0 , 9 6 * 3 2 * 4 ) ;  
  
 	 	 	 i f   ( n u m F r a m e s > 0 )   / /   J u s t   u s e   t h e   f i r s t   o n e  
 	 	 	 {  
 	 	 	 	 u 3 2   * i c d a t a   =   a n i m D a t a ;  
  
 	 	 	 	 f o r   ( i n t   y = 0 ; y < 3 2 ; y + + )  
 	 	 	 	 {  
 	 	 	 	 	 f o r   ( i n t   x = 0 ; x < 3 2 ; x + + )  
 	 	 	 	 	 {  
 	 	 	 	 	 	 p x d a t a [ y * 9 6 + x + 3 2 ]   =   i c d a t a [ y * 3 2 + x ] ; / /     |   0 x F F 0 0 0 0 0 0  
 	 	 	 	 	 }  
 	 	 	 	 }  
 	 	 	 }  
 	 	 }  
  
 	 	 w x B i t m a p   m a p   =   w x B i t m a p F r o m M e m o r y R G B A ( ( u 8 * ) p x d a t a , 9 6 , 3 2 ) ;  
 	 	 i m a g e s [ i * 2 ]   =   l i s t - > A d d ( m a p ) ;  
  
 	 	 i f   ( n u m F r a m e s > 0 )  
 	 	 {  
 	 	 	 m e m s e t ( p x d a t a , 0 , 9 6 * 3 2 * 4 ) ;  
 	 	 	 i n t   f r a m e s = 3 ;  
 	 	 	 i f   ( n u m F r a m e s < f r a m e s )   f r a m e s = n u m F r a m e s ;  
 	 	 	 f o r   ( i n t   f = 0 ; f < f r a m e s ; f + + )  
 	 	 	 {  
 	 	 	 	 f o r   ( i n t   y = 0 ; y < 3 2 ; y + + )  
 	 	 	 	 {  
 	 	 	 	 	 f o r   ( i n t   x = 0 ; x < 3 2 ; x + + )  
 	 	 	 	 	 {  
 	 	 	 	 	 	 p x d a t a [ y * 9 6   +   x   +   3 2 * f ]   =   a n i m D a t a [ f * 3 2 * 3 2   +   y * 3 2   +   x ] ;  
 	 	 	 	 	 }  
 	 	 	 	 }  
 	 	 	 }  
 	 	 	 w x B i t m a p   i c o n   =   w x B i t m a p F r o m M e m o r y R G B A ( ( u 8 * ) p x d a t a , 9 6 , 3 2 ) ;  
 	 	 	 i m a g e s [ i * 2 + 1 ]   =   l i s t - > A d d ( i c o n ) ;  
 	 	 }  
 	 }  
  
 	 i n t 	 p a g e s M a x   =   ( m c m S e t t i n g s . u s e P a g e s )   ?  
 	 	 	 	 	 ( p a g e [ c a r d ]   +   1 )   *   i t e m s P e r P a g e   :   1 2 8 ;  
  
 	 f o r   ( j   =   p a g e [ c a r d ]   *   i t e m s P e r P a g e ;   ( j   <   n F i l e s )   & &   ( j   <   p a g e s M a x ) ;   j + + )  
 	 {  
 	 	 c h a r   t i t l e [ D E N T R Y _ S T R L E N ] ;  
 	 	 c h a r   c o m m e n t [ D E N T R Y _ S T R L E N ] ;  
 	 	 u 1 6   b l o c k s ;  
 	 	 u 1 6   f i r s t b l o c k ;  
  
 	 	 i n t   i n d e x   =   m _ M e m c a r d L i s t [ c a r d ] - > I n s e r t I t e m ( j ,   w x E m p t y S t r i n g ) ;  
  
 	 	 m _ M e m c a r d L i s t [ c a r d ] - > S e t I t e m ( i n d e x ,   C O L U M N _ B A N N E R ,   w x E m p t y S t r i n g ) ;  
  
 	 	 i f   ( ! m e m o r y C a r d [ c a r d ] - > D E n t r y _ C o m m e n t 1 ( j ,   t i t l e ) )   t i t l e [ 0 ] = 0 ;  
 	 	 i f   ( ! m e m o r y C a r d [ c a r d ] - > D E n t r y _ C o m m e n t 2 ( j ,   c o m m e n t ) )   c o m m e n t [ 0 ] = 0 ;  
  
 	 	 b o o l   a s c i i   =   m e m o r y C a r d [ c a r d ] - > I s A s c i i E n c o d i n g ( ) ;  
 # i f d e f   _ _ l i n u x _ _  
 	 	 w x C S C o n v   S J I S C o n v ( w x F o n t M a p p e r : : G e t E n c o d i n g N a m e ( w x F O N T E N C O D I N G _ E U C _ J P ) ) ;  
 # e l s e  
 	 	 w x C S C o n v   S J I S C o n v ( w x F o n t M a p p e r : : G e t E n c o d i n g N a m e ( w x F O N T E N C O D I N G _ S H I F T _ J I S ) ) ;  
 # e n d i f  
 	 	 w x T i t l e     =     w x S t r i n g ( t i t l e ,   a s c i i   ?   * w x C o n v C u r r e n t   :   S J I S C o n v ) ;  
 	 	 w x C o m m e n t   =   w x S t r i n g ( c o m m e n t ,   a s c i i   ?   * w x C o n v C u r r e n t   :   S J I S C o n v ) ;  
  
 	 	 m _ M e m c a r d L i s t [ c a r d ] - > S e t I t e m ( i n d e x ,   C O L U M N _ T I T L E ,   w x T i t l e ) ;  
 	 	 m _ M e m c a r d L i s t [ c a r d ] - > S e t I t e m ( i n d e x ,   C O L U M N _ C O M M E N T ,   w x C o m m e n t ) ;  
  
 	 	 b l o c k s   =   m e m o r y C a r d [ c a r d ] - > D E n t r y _ B l o c k C o u n t ( j ) ;  
 	 	 i f   ( b l o c k s   = =   0 x F F F F )   b l o c k s   =   0 ;  
 	 	 w x B l o c k . P r i n t f ( w x T ( " % 1 0 d " ) ,   b l o c k s ) ;  
 	 	 m _ M e m c a r d L i s t [ c a r d ] - > S e t I t e m ( i n d e x , C O L U M N _ B L O C K S ,   w x B l o c k ) ;  
 	 	 f i r s t b l o c k   =   m e m o r y C a r d [ c a r d ] - > D E n t r y _ F i r s t B l o c k ( j ) ;  
 	 	 i f   ( f i r s t b l o c k   = =   0 x F F F F )   f i r s t b l o c k   =   3 ; 	 / /   t o   m a k e   f i r s t b l o c k   - 1  
 	 	 w x F i r s t B l o c k . P r i n t f ( w x T ( " % 1 5 d " ) ,   f i r s t b l o c k - 4 ) ;  
 	 	 m _ M e m c a r d L i s t [ c a r d ] - > S e t I t e m ( i n d e x , C O L U M N _ F I R S T B L O C K ,   w x F i r s t B l o c k ) ;  
 	 	 m _ M e m c a r d L i s t [ c a r d ] - > S e t I t e m ( i n d e x ,   C O L U M N _ I C O N ,   w x E m p t y S t r i n g ) ;  
  
 	 	 i f   ( i m a g e s [ j ]   > =   0 )  
 	 	 {  
 	 	 	 m _ M e m c a r d L i s t [ c a r d ] - > S e t I t e m I m a g e ( i n d e x ,   i m a g e s [ j * 2 ] ) ;  
 	 	 	 m _ M e m c a r d L i s t [ c a r d ] - > S e t I t e m C o l u m n I m a g e ( i n d e x ,   C O L U M N _ I C O N ,   i m a g e s [ j * 2 + 1 ] ) ;  
 	 	 }  
 	 }  
  
 	 i f   ( m c m S e t t i n g s . u s e P a g e s )  
 	 {  
 	 	 i f   ( n F i l e s   < =   i t e m s P e r P a g e )  
 	 	 {  
 	 	 	 m _ P r e v P a g e [ c a r d ] - > D i s a b l e ( ) ;  
 	 	 	 m _ M e m c a r d L i s t [ c a r d ] - > p r e v P a g e   =   f a l s e ;  
 	 	 }  
 	 	 i f   ( j   = =   n F i l e s )  
 	 	 {  
 	 	 	 m _ N e x t P a g e [ c a r d ] - > D i s a b l e ( ) ;  
 	 	 	 m _ M e m c a r d L i s t [ c a r d ] - > n e x t P a g e   =   f a l s e ;  
 	 	 }  
 	 	 e l s e  
 	 	 {  
 	 	 	 m _ N e x t P a g e [ c a r d ] - > E n a b l e ( ) ;  
 	 	 	 m _ M e m c a r d L i s t [ c a r d ] - > n e x t P a g e   =   t r u e ;  
 	 	 }  
 	 }  
  
 	 d e l e t e [ ]   i m a g e s ;  
 	 / /   A u t o m a t i c   c o l u m n   w i d t h   a n d   t h e n   s h o w   t h e   l i s t  
 	 f o r   ( i n t   i   =   0 ;   i   <   N U M B E R _ O F _ C O L U M N ;   i + + )  
 	 {  
 	 	 i f   ( m c m S e t t i n g s . c o l u m n [ i ] )  
 	 	 	 m _ M e m c a r d L i s t [ c a r d ] - > S e t C o l u m n W i d t h ( i ,   w x L I S T _ A U T O S I Z E ) ;  
 	 	 e l s e  
 	 	 	 m _ M e m c a r d L i s t [ c a r d ] - > S e t C o l u m n W i d t h ( i ,   0 ) ;  
 	 }  
  
 	 m _ M e m c a r d L i s t [ c a r d ] - > S h o w ( ) ;  
 	 w x L a b e l . P r i n t f ( w x T ( " % d   F r e e   B l o c k s ;   % d   F r e e   D i r   E n t r i e s " ) ,  
 	 	 m e m o r y C a r d [ c a r d ] - > G e t F r e e B l o c k s ( ) ,   D I R L E N   -   n F i l e s ) ;  
 	 t _ S t a t u s [ c a r d ] - > S e t L a b e l ( w x L a b e l ) ;  
  
 	 r e t u r n   t r u e ;  
 }  
  
 v o i d   C M e m c a r d M a n a g e r : : C M e m c a r d L i s t C t r l : : O n R i g h t C l i c k ( w x M o u s e E v e n t &   e v e n t )  
 {  
  
 	 i n t   f l a g s ;  
 	 l o n g   i t e m   =   H i t T e s t ( e v e n t . G e t P o s i t i o n ( ) ,   f l a g s ) ;  
 	 w x M e n u *   p o p u p M e n u   =   n e w   w x M e n u ;  
  
 	 i f   ( i t e m   ! =   w x N O T _ F O U N D )    
 	 {  
 	 	 i f   ( G e t I t e m S t a t e ( i t e m ,   w x L I S T _ S T A T E _ S E L E C T E D )   ! =   w x L I S T _ S T A T E _ S E L E C T E D )  
 	 	 {  
 	 	 	 S e t I t e m S t a t e ( i t e m ,   w x L I S T _ S T A T E _ S E L E C T E D ,   w x L I S T _ S T A T E _ S E L E C T E D   ) ;  
 	 	 }  
 	 	 S e t I t e m S t a t e ( i t e m ,   w x L I S T _ S T A T E _ F O C U S E D ,   w x L I S T _ S T A T E _ F O C U S E D ) ;  
  
 	 	 i n t   s l o t   =   G e t I d ( )   -   I D _ M E M C A R D L I S T _ A ;  
 	 	 p o p u p M e n u - > A p p e n d ( I D _ C O P Y F R O M _ A   +   s l o t ,   w x S t r i n g : : F o r m a t ( w x T ( " Y6R0R�QX[aS  % c " ) ,   ' B '   -   s l o t ) ) ;  
 	 	 p o p u p M e n u - > A p p e n d ( I D _ D E L E T E _ A   +   s l o t ,   w x T ( "  Rd�X[ch" ) ) ;  
 	 	 p o p u p M e n u - > A p p e n d ( I D _ S A V E I M P O R T _ A   +   s l o t ,   w x T ( " �[eQX[ch" ) ) ;  
 	 	 p o p u p M e n u - > A p p e n d ( I D _ S A V E E X P O R T _ A   +   s l o t ,   w x T ( " �[�QX[ch" ) ) ;  
 	 	 p o p u p M e n u - > A p p e n d ( I D _ E X P O R T A L L _ A   +   s l o t ,   w x T ( " �[�Q@b	gX[ch" ) ) ;  
 	 	 	 	  
 	 	 p o p u p M e n u - > F i n d I t e m ( I D _ C O P Y F R O M _ A   +   s l o t ) - > E n a b l e ( _ _ m c m S e t t i n g s . t w o C a r d s L o a d e d ) ;  
  
 	 	 p o p u p M e n u - > A p p e n d S e p a r a t o r ( ) ;  
  
 	 	 p o p u p M e n u - > A p p e n d ( I D _ F I X C H E C K S U M _ A   +   s l o t ,   w x T ( " �Ock!h�h�T" ) ) ;  
 	 	 p o p u p M e n u - > A p p e n d ( I D _ P R E V P A G E _ A   +   s l o t ,   w x T ( " 
N Nu�" ) ) ;  
 	 	 p o p u p M e n u - > A p p e n d ( I D _ N E X T P A G E _ A   +   s l o t ,   w x T ( " N Nu�" ) ) ;  
 	 	 p o p u p M e n u - > A p p e n d ( I D _ M E M C A R D P A T H _ A   +   s l o t ,   w x S t r i n g : : F o r m a t ( w x T ( " ��n:N؞���QX[aS  % c " ) ,   ' A '   +   s l o t ) ) ;  
 	 	 p o p u p M e n u - > A p p e n d C h e c k I t e m ( I D _ U S E P A G E S ,   w x T ( " E n a b l e   p a g e s " ) ) ;  
 	  
 	 	 p o p u p M e n u - > F i n d I t e m ( I D _ P R E V P A G E _ A   +   s l o t ) - > E n a b l e ( p r e v P a g e   & &   _ _ m c m S e t t i n g s . u s e P a g e s ) ;  
 	 	 p o p u p M e n u - > F i n d I t e m ( I D _ N E X T P A G E _ A   +   s l o t ) - > E n a b l e ( n e x t P a g e   & &   _ _ m c m S e t t i n g s . u s e P a g e s ) ;  
 	 	 p o p u p M e n u - > F i n d I t e m ( I D _ U S E P A G E S ) - > C h e c k ( _ _ m c m S e t t i n g s . u s e P a g e s ) ;  
 	 	  
 	 	 p o p u p M e n u - > A p p e n d S e p a r a t o r ( ) ;  
  
 	 	 p o p u p M e n u - > A p p e n d C h e c k I t e m ( C O L U M N _ B A N N E R ,   w x T ( " >f:yX[chL O G O " ) ) ;  
 	 	 p o p u p M e n u - > A p p e n d C h e c k I t e m ( C O L U M N _ T I T L E ,   w x T ( " >f:yX[chh��" ) ) ;  
 	 	 p o p u p M e n u - > A p p e n d C h e c k I t e m ( C O L U M N _ C O M M E N T ,   w x T ( " S h o w   s a v e   c o m m e n t " ) ) ;  
 	 	 p o p u p M e n u - > A p p e n d C h e c k I t e m ( C O L U M N _ I C O N ,   w x T ( " >f:yX[ch�Vh" ) ) ;  
 	 	 p o p u p M e n u - > A p p e n d C h e c k I t e m ( C O L U M N _ B L O C K S ,   w x T ( " S h o w   s a v e   b l o c k s " ) ) ;  
  
 	 	 f o r   ( i n t   i   =   C O L U M N _ B A N N E R ;   i   < =   C O L U M N _ B L O C K S ;   i + + )  
 	 	 {  
 	 	 	 p o p u p M e n u - > F i n d I t e m ( i ) - > C h e c k ( _ _ m c m S e t t i n g s . c o l u m n [ i ] ) ;  
 	 	 }  
 	 }  
 	 P o p u p M e n u ( p o p u p M e n u ) ;  
 }  
  
 