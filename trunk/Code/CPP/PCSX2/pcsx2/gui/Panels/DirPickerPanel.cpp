/ *     P C S X 2   -   P S 2   E m u l a t o r   f o r   P C s  
   *     C o p y r i g h t   ( C )   2 0 0 2 - 2 0 1 0     P C S X 2   D e v   T e a m  
   *  
   *     P C S X 2   i s   f r e e   s o f t w a r e :   y o u   c a n   r e d i s t r i b u t e   i t   a n d / o r   m o d i f y   i t   u n d e r   t h e   t e r m s  
   *     o f   t h e   G N U   L e s s e r   G e n e r a l   P u b l i c   L i c e n s e   a s   p u b l i s h e d   b y   t h e   F r e e   S o f t w a r e   F o u n d -  
   *     a t i o n ,   e i t h e r   v e r s i o n   3   o f   t h e   L i c e n s e ,   o r   ( a t   y o u r   o p t i o n )   a n y   l a t e r   v e r s i o n .  
   *  
   *     P C S X 2   i s   d i s t r i b u t e d   i n   t h e   h o p e   t h a t   i t   w i l l   b e   u s e f u l ,   b u t   W I T H O U T   A N Y   W A R R A N T Y ;  
   *     w i t h o u t   e v e n   t h e   i m p l i e d   w a r r a n t y   o f   M E R C H A N T A B I L I T Y   o r   F I T N E S S   F O R   A   P A R T I C U L A R  
   *     P U R P O S E .     S e e   t h e   G N U   G e n e r a l   P u b l i c   L i c e n s e   f o r   m o r e   d e t a i l s .  
   *  
   *     Y o u   s h o u l d   h a v e   r e c e i v e d   a   c o p y   o f   t h e   G N U   G e n e r a l   P u b l i c   L i c e n s e   a l o n g   w i t h   P C S X 2 .  
   *     I f   n o t ,   s e e   < h t t p : / / w w w . g n u . o r g / l i c e n s e s / > .  
   * /  
  
 # i n c l u d e   " P r e c o m p i l e d H e a d e r . h "  
 # i n c l u d e   " C o n f i g u r a t i o n P a n e l s . h "  
 # i n c l u d e   " D i a l o g s / M o d a l P o p u p s . h "  
  
 # i n c l u d e   < w x / s t d p a t h s . h >  
 # i n c l u d e   < w x / f i l e . h >  
 # i n c l u d e   < w x / d i r . h >  
 # i n c l u d e   < w x / f i l e p i c k e r . h >  
  
 u s i n g   n a m e s p a c e   p x S i z e r F l a g s ;  
  
 s t a t i c   w x S t r i n g   G e t N o r m a l i z e d C o n f i g F o l d e r (   F o l d e r s E n u m _ t   f o l d e r I d   )  
 {  
 	 r e t u r n   P a t h : : N o r m a l i z e (   g _ C o n f - > F o l d e r s . I s D e f a u l t (   f o l d e r I d   )   ?   P a t h D e f s : : G e t ( f o l d e r I d )   :   g _ C o n f - > F o l d e r s [ f o l d e r I d ]   ) ;  
 }  
  
 / /   P a s s   m e   T R U E   i f   t h e   d e f a u l t   p a t h   i s   t o   b e   u s e d ,   a n d   t h e   D i r P i c k e r C t r l   d i s a b l e d   f r o m   u s e .  
 v o i d   P a n e l s : : D i r P i c k e r P a n e l : : U p d a t e C h e c k S t a t u s (   b o o l   s o m e N o t e w o r t h y B o o l e a n   )  
 {  
 	 m _ p i c k e r C t r l - > E n a b l e (   ! s o m e N o t e w o r t h y B o o l e a n   ) ;  
 	 i f (   s o m e N o t e w o r t h y B o o l e a n   )  
 	 {  
 	 	 w x S t r i n g   n o r m a l i z e d (   P a t h : : N o r m a l i z e (   P a t h D e f s : : G e t (   m _ F o l d e r I d   )   )   ) ;  
 	 	 m _ p i c k e r C t r l - > S e t P a t h (   n o r m a l i z e d   ) ;  
  
 	 	 w x F i l e D i r P i c k e r E v e n t   e v e n t (   m _ p i c k e r C t r l - > G e t E v e n t T y p e ( ) ,   m _ p i c k e r C t r l ,   m _ p i c k e r C t r l - > G e t I d ( ) ,   n o r m a l i z e d   ) ;  
 	 	 m _ p i c k e r C t r l - > G e t E v e n t H a n d l e r ( ) - > P r o c e s s E v e n t ( e v e n t ) ;  
 	 }  
 }  
  
 v o i d   P a n e l s : : D i r P i c k e r P a n e l : : U s e D e f a u l t P a t h _ C l i c k (   w x C o m m a n d E v e n t   & e v t   )  
 {  
 	 e v t . S k i p ( ) ;  
 	 p x A s s e r t (   ( m _ p i c k e r C t r l   ! =   N U L L )   & &   ( m _ c h e c k C t r l   ! =   N U L L )   ) ;  
 	 U p d a t e C h e c k S t a t u s (   m _ c h e c k C t r l   ?   m _ c h e c k C t r l - > I s C h e c k e d ( )   :   f a l s e   ) ;  
 }  
  
 v o i d   P a n e l s : : D i r P i c k e r P a n e l : : E x p l o r e _ C l i c k (   w x C o m m a n d E v e n t   & e v t   )  
 {  
 	 w x S t r i n g   p a t h (   m _ p i c k e r C t r l - > G e t P a t h ( )   ) ;  
 	 i f (   ! w x D i r E x i s t s ( p a t h )   )  
 	 {  
 	 	 w x D i a l o g W i t h H e l p e r s   c r e a t e P a t h D l g (   N U L L ,   _ ( " _NX[(W" )   ) ;  
 	 	 c r e a t e P a t h D l g . S e t M i n W i d t h (   6 0 0   ) ;  
  
 	 	 c r e a t e P a t h D l g   + =   c r e a t e P a t h D l g . T e x t (   p a t h   )   |   S t d C e n t e r ( ) ;  
  
 	 	 c r e a t e P a t h D l g   + =   c r e a t e P a t h D l g . H e a d i n g (   p x E (   " ! N o t i c e : D i r P i c k e r : C r e a t e P a t h " ,  
 	 	 	 L " c�[�v_/ �vU_NX[(W.     /f&T ���R�^ N*N? "   )  
 	 	 ) ;  
  
 	 	 w x W i n d o w I D   r e s u l t   =   p x I s s u e C o n f i r m a t i o n (   c r e a t e P a t h D l g ,  
 	 	 	 M s g B u t t o n s ( ) . C u s t o m ( _ ( " R�^" ) ,   " c r e a t e " ) . C a n c e l ( ) ,  
 	 	 	 L " D i r P i c k e r : C r e a t e O n E x p l o r e "  
 	 	 ) ;  
  
 	 	 i f (   r e s u l t   = =   w x I D _ C A N C E L   )   r e t u r n ;  
 	 	 w x D i r N a m e ( p a t h ) . M k d i r ( ) ;  
 	 }  
  
 	 p x E x p l o r e (   p a t h   ) ;  
 }  
  
 / /   - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -  
 / /   I f   i n i t P a t h   i s   N U L L ,   t h e n   i t ' s   a s s u m e d   t h e   d e f a u l t   f o l d e r   i s   t o   b e   u s e d ,   w h i c h   i s  
 / /   o b t a i n e d   f r o m   i n v o k i n g   t h e   s p e c i f i e d   g e t D e f a u l t ( )   f u n c t i o n .  
 / /  
 P a n e l s : : D i r P i c k e r P a n e l : : D i r P i c k e r P a n e l (   w x W i n d o w *   p a r e n t ,   F o l d e r s E n u m _ t   f o l d e r i d ,   c o n s t   w x S t r i n g &   l a b e l ,   c o n s t   w x S t r i n g &   d i a l o g L a b e l   )  
 	 :   B a s e A p p l i c a b l e C o n f i g P a n e l (   p a r e n t ,   w x V E R T I C A L ,   l a b e l   )  
 {  
 	 I n i t (   f o l d e r i d ,   d i a l o g L a b e l ,   f a l s e   ) ;  
 }  
  
 P a n e l s : : D i r P i c k e r P a n e l : : D i r P i c k e r P a n e l (   w x W i n d o w *   p a r e n t ,   F o l d e r s E n u m _ t   f o l d e r i d ,   c o n s t   w x S t r i n g &   d i a l o g L a b e l   )  
 	 :   B a s e A p p l i c a b l e C o n f i g P a n e l (   p a r e n t ,   w x V E R T I C A L   )  
 {  
 	 I n i t (   f o l d e r i d ,   d i a l o g L a b e l ,   t r u e   ) ;  
 }  
  
 v o i d   P a n e l s : : D i r P i c k e r P a n e l : : I n i t (   F o l d e r s E n u m _ t   f o l d e r i d ,   c o n s t   w x S t r i n g &   d i a l o g L a b e l ,   b o o l   i s C o m p a c t   )  
 {  
 	 m _ F o l d e r I d 	 	 =   f o l d e r i d ;  
 	 m _ p i c k e r C t r l 	 =   N U L L ;  
 	 m _ c h e c k C t r l 	 	 =   N U L L ;  
  
 	 / /   F o r c e   t h e   D i r   P i c k e r   t o   u s e   a   t e x t   c o n t r o l .     T h i s   i s n ' t   s t a n d a r d   o n   L i n u x / G T K   b u t   i t ' s   m u c h  
 	 / /   m o r e   u s a b l e ,   s o   t o   h e l l   w i t h   s t a n d a r d s .  
  
 	 w x S t r i n g   n o r m a l i z e d (   G e t N o r m a l i z e d C o n f i g F o l d e r (   m _ F o l d e r I d   )   ) ;  
  
 	 i f (   w x F i l e : : E x i s t s (   n o r m a l i z e d   )   )  
 	 {  
 	 	 / /   T h e   d e f a u l t   p a t h   i s   i n v a l i d . . .   W h a t   s h o u l d   w e   d o   h e r e ?   h m m . .  
 	 }  
  
 	 / / i f (   ! w x D i r : : E x i s t s (   n o r m a l i z e d   )   )  
 	 / / 	 w x M k d i r (   n o r m a l i z e d   ) ;  
  
 	 i f (   ! i s C o m p a c t   )  
 	 {  
 	 	 m _ c h e c k C t r l   =   n e w   p x C h e c k B o x (   t h i s ,   _ ( " O(u؞����n" )   ) ;  
  
 	 	 p x S e t T o o l T i p (   m _ c h e c k C t r l ,   p x E t (   " ! C o n t e x t T i p : D i r P i c k e r : U s e D e f a u l t " ,  
 	 	 	 L " W h e n   c h e c k e d   t h i s   f o l d e r   w i l l   a u t o m a t i c a l l y   r e f l e c t   t h e   d e f a u l t   a s s o c i a t e d   w i t h   P C S X 2 ' s   c u r r e n t   u s e r m o d e   s e t t i n g .   "   )  
 	 	 ) ;  
  
 	 	 C o n n e c t (   m _ c h e c k C t r l - > G e t I d ( ) , 	 w x E V T _ C O M M A N D _ C H E C K B O X _ C L I C K E D ,   w x C o m m a n d E v e n t H a n d l e r (   D i r P i c k e r P a n e l : : U s e D e f a u l t P a t h _ C l i c k   )   ) ;  
 	 }  
  
 	 m _ p i c k e r C t r l   =   n e w   w x D i r P i c k e r C t r l (   t h i s ,   w x I D _ A N Y ,   w x E m p t y S t r i n g ,   d i a l o g L a b e l ,  
 	 	 w x D e f a u l t P o s i t i o n ,   w x D e f a u l t S i z e ,   w x D I R P _ U S E _ T E X T C T R L   |   w x D I R P _ D I R _ M U S T _ E X I S T  
 	 ) ;  
  
 # i f n d e f   _ _ W X G T K _ _  
 	 / /   G T K +   :   T h e   w x   i m p l e m e n t a t i o n   o f   E x p l o r e   i s n ' t   r e l i a b l e ,   s o   l e t ' s   n o t   e v e n   p u t   t h e  
 	 / /   b u t t o n   o n   t h e   d i a l o g s   f o r   n o w .  
  
 	 w x B u t t o n *   b _ e x p l o r e (   n e w   w x B u t t o n (   t h i s ,   w x I D _ A N Y ,   _ ( " (uD��n�{thVSb _" )   )   ) ;  
 	 p x S e t T o o l T i p (   b _ e x p l o r e ,   _ ( " O(uD��n�{thVSb _ُ*N�e�N9Y. " )   ) ;  
 	 C o n n e c t (   b _ e x p l o r e - > G e t I d ( ) , 	 w x E V T _ C O M M A N D _ B U T T O N _ C L I C K E D , 	 w x C o m m a n d E v e n t H a n d l e r (   D i r P i c k e r P a n e l : : E x p l o r e _ C l i c k   )   ) ;  
 # e n d i f  
  
 	 i f (   i s C o m p a c t   )  
 	 {  
 	 	 w x F l e x G r i d S i z e r &   s _ c o m p a c t (   * n e w   w x F l e x G r i d S i z e r (   2 ,   0 ,   4   )   ) ;  
 	 	 s _ c o m p a c t . A d d G r o w a b l e C o l (   0   ) ;  
 	 	 s _ c o m p a c t   + =   m _ p i c k e r C t r l   |   p x E x p a n d ;  
 # i f n d e f   _ _ W X G T K _ _  
 	 	 s _ c o m p a c t   + =   b _ e x p l o r e ;  
 # e n d i f  
 	 	 * t h i s   + =   s _ c o m p a c t 	 |   p x E x p a n d ;   / / . B o r d e r ( w x L E F T   |   w x R I G H T   |   w x T O P ,   5 ) ;  
 	 }  
 	 e l s e  
 	 {  
 	 	 w x B o x S i z e r &   s _ l o w e r (   * n e w   w x B o x S i z e r (   w x H O R I Z O N T A L   )   ) ;  
  
 	 	 s _ l o w e r   + =   m _ c h e c k C t r l 	 |   p x M i d d l e ;  
 # i f n d e f   _ _ W X G T K _ _  
 	 	 s _ l o w e r   + =   p x S t r e t c h S p a c e r ( 1 ) ;  
 	 	 s _ l o w e r   + =   b _ e x p l o r e ;  
 # e n d i f  
  
 	 	 * t h i s   + =   m _ p i c k e r C t r l 	 |   p x E x p a n d . B o r d e r ( w x L E F T   |   w x R I G H T   |   w x T O P ,   S t d P a d d i n g ) ;  
 	 	 * t h i s   + =   s _ l o w e r 	 	 |   p x E x p a n d . B o r d e r ( w x L E F T   |   w x R I G H T ,   S t d P a d d i n g ) ;  
 	 }  
  
 	 / /   w x   w a r n s   w h e n   p a t h s   d o n ' t   e x i s t ,   b u t   t h i s   i s   t y p i c a l l y   n o r m a l   w h e n   t h e   w i z a r d  
 	 / /   c r e a t e s   i t s   c h i l d   c o n t r o l s .     S o   l e t ' s   i g n o r e   t h e m .  
 	 w x D o N o t L o g I n T h i s S c o p e   p l e a s e ;  
 	 A p p S t a t u s E v e n t _ O n S e t t i n g s A p p l i e d ( ) ; 	 / /   f o r c e s   d e f a u l t   s e t t i n g s   b a s e d   o n   g _ C o n f  
 }  
  
 P a n e l s : : D i r P i c k e r P a n e l &   P a n e l s : : D i r P i c k e r P a n e l : : S e t S t a t i c D e s c (   c o n s t   w x S t r i n g &   m s g   )  
 {  
 	 G e t S i z e r ( ) - > I n s e r t (   0 ,   & H e a d i n g (   m s g   ) ,   p x E x p a n d   ) ;  
 	 r e t u r n   * t h i s ;  
 }  
  
 P a n e l s : : D i r P i c k e r P a n e l &   P a n e l s : : D i r P i c k e r P a n e l : : S e t T o o l T i p (   c o n s t   w x S t r i n g &   t i p   )  
 {  
 	 p x S e t T o o l T i p (   t h i s ,   t i p   ) ;  
 	 r e t u r n   * t h i s ;  
 }  
  
 w x W i n d o w I D   P a n e l s : : D i r P i c k e r P a n e l : : G e t I d ( )   c o n s t  
 {  
 	 r e t u r n   m _ p i c k e r C t r l - > G e t I d ( ) ;  
 }  
  
 v o i d   P a n e l s : : D i r P i c k e r P a n e l : : R e s e t ( )  
 {  
 	 c o n s t   b o o l   i s D e f a u l t   =   g _ C o n f - > F o l d e r s . I s D e f a u l t (   m _ F o l d e r I d   ) ;  
  
 	 i f (   m _ c h e c k C t r l   )  
 	 	 m _ c h e c k C t r l - > S e t V a l u e (   i s D e f a u l t   ) ;  
  
 	 i f (   m _ p i c k e r C t r l   )  
 	 {  
 	 	 / /   I m p o r t a n t !     T h e   d i r p i c k e r   p a n e l   s t u f f ,   d u e   t o   h o w e v e r   i t ' s   p u t   t o g e t h e r  
 	 	 / /   n e e d s   t o   c h e c k   t h e   e n a b l e   s t a t u s   o f   t h i s   p a n e l   b e f o r e   s e t t i n g   t h e   c h i l d  
 	 	 / /   p a n e l ' s   e n a b l e   s t a t u s .  
  
 	 	 m _ p i c k e r C t r l - > E n a b l e (   I s E n a b l e d ( )   ?   (   m _ c h e c k C t r l   ?   ! i s D e f a u l t   :   t r u e   )   :   f a l s e   ) ;  
 	 	 m _ p i c k e r C t r l - > S e t P a t h (   G e t N o r m a l i z e d C o n f i g F o l d e r (   m _ F o l d e r I d   )   ) ;  
 	 }  
 }  
  
 b o o l   P a n e l s : : D i r P i c k e r P a n e l : : E n a b l e (   b o o l   e n a b l e   )  
 {  
 	 m _ p i c k e r C t r l - > E n a b l e (   e n a b l e   ?   ( ! m _ c h e c k C t r l   | |   m _ c h e c k C t r l - > G e t V a l u e ( ) )   :   f a l s e   ) ;  
 	 r e t u r n   _ p a r e n t : : E n a b l e (   e n a b l e   ) ;  
 }  
  
  
 v o i d   P a n e l s : : D i r P i c k e r P a n e l : : A p p S t a t u s E v e n t _ O n S e t t i n g s A p p l i e d ( )  
 {  
 	 R e s e t ( ) ;  
 }  
  
 v o i d   P a n e l s : : D i r P i c k e r P a n e l : : A p p l y ( )  
 {  
 	 i f (   ! m _ p i c k e r C t r l   )   r e t u r n ;  
  
 	 c o n s t   w x S t r i n g   p a t h (   m _ p i c k e r C t r l - > G e t P a t h ( )   ) ;  
  
 	 i f (   ! w x D i r : : E x i s t s (   p a t h   )   )  
 	 {  
 	 	 w x D i a l o g W i t h H e l p e r s   d i a l o g (   N U L L ,   _ ( " R�^�e�N9Y? " )   ) ;  
 	 	 d i a l o g   + =   d i a l o g . H e a d i n g ( A d d A p p N a m e ( _ ( " A   c o n f i g u r e d   f o l d e r   d o e s   n o t   e x i s t .     S h o u l d   % s   t r y   t o   c r e a t e   i t ? " ) ) ) ;  
 	 	 d i a l o g   + =   1 2 ;  
 	 	 d i a l o g   + =   d i a l o g . H e a d i n g (   p a t h   ) ;  
  
 	 	 i f (   w x I D _ C A N C E L   = =   p x I s s u e C o n f i r m a t i o n (   d i a l o g ,   M s g B u t t o n s ( ) . C u s t o m ( _ ( " R�^" ) ,   " c r e a t e " ) . C a n c e l ( ) ,   L " C r e a t e N e w F o l d e r "   )   )  
 	 	 	 t h r o w   E x c e p t i o n : : C a n n o t A p p l y S e t t i n g s (   t h i s   ) ;  
 	 }  
  
 	 w x D i r N a m e ( p a t h ) . M k d i r ( ) ;  
 	 g _ C o n f - > F o l d e r s . S e t (   m _ F o l d e r I d ,   m _ p i c k e r C t r l - > G e t P a t h ( ) ,   m _ c h e c k C t r l   ?   m _ c h e c k C t r l - > G e t V a l u e ( )   :   f a l s e   ) ;  
 }  
  
 w x D i r N a m e   P a n e l s : : D i r P i c k e r P a n e l : : G e t P a t h ( )   c o n s t  
 {  
 	 / /   T h e   ( x )   ?   y   :   z   c o n s t r u c t   d o e s n ' t   l i k e   y   a n d   z   t o   b e   d i f f e r e n t   t y p e s   i n   g c c .  
 	 i f   ( m _ p i c k e r C t r l )  
 	 	 r e t u r n   w x D i r N a m e ( m _ p i c k e r C t r l - > G e t P a t h ( ) ) ;  
 	 e l s e  
 	 	 r e t u r n   w x D i r N a m e ( w x E m p t y S t r i n g ) ;  
 }  
  
 v o i d   P a n e l s : : D i r P i c k e r P a n e l : : S e t P a t h (   c o n s t   w x S t r i n g &   n e w P a t h   )  
 {  
 	 m _ p i c k e r C t r l - > S e t P a t h (   n e w P a t h   ) ;  
 }  
 