// O Código 7 calcula o valor errado para SIZE. Por que?

// # i n c l u d e < s t d i o . h>
// # d e f i n e SIZE 1 0 ;
// # d e f i n e FUDGE SIZE −2;
// i n t main ( ) {
// int size ;
// s i z e = FUDGE;
// p r i n t f ( " S i z e i s %d\n " , s i z e ) ;
// return ( 0 ) ;
// }

// O erro é por causa do ";" q está fazendo a subistituição com 10; e -2; o certo seria só tirar os ";". 