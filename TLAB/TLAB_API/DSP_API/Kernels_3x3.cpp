///         http://setosa.io/ev/image-kernels/
///         http://matlabtricks.com/post-5/3x3-convolution-kernels-with-online-demo
// Kernel de reducción de ruido.
const int ConvKernel[9] = {
                        1,     1,      1,
                        1,     1,      1,
                        1,     1,      1
                       };
///
int ConvKernelMediaLow[9] = {
                        1,     1,      1,
                        1,     1,      1,
                        1,     1,      1
                             };
///
int ConvKernelMediaLowPon[9] = {
                        1,     1,      1,
                        1,     2,      1,
                        1,     1,      1
                             };
///
int ConvKernelMediaHigh[9] = {
                        -1,     -1,      -1,
                        -1,      8,      -1,
                        -1,     -1,      -1
                             };
///
int ConvKernelLaplacHigh[9] = {
                        0,     1,      0,
                        1,    -4,      1,
                        0,     1,      0
                             };
///
int ConvKernelLaplacHighMin[9] = {
                         0,     -1,      0,
                        -1,      5,     -1,
                         0,     -1,      0
                             };
///
///
///
///
int ConvKernelBlur[9] = {
                        1,     2,      1,
                        2,     0,      2,
                        1,     2,      1
                             };
///

// Kernel de Realzado, Emboss.
//*
int ConvKernelEmboss[9] = {
                        2,     0,      0,
                        0,    -1,      0,
                        0,     0,     -1
                             };
//*/
/*
int ConvKernelEmboss[9] = {
                        -2,    -1,      0,
                        -1,     1,      1,
                         0,     1,      2
                             };
//*/
///



/* Kernel de Nitidez, Sharpen. *//*ConvKernelSharpen[1][1] = Mod;*/ /* Default de Mod=5; */
int ConvKernelSharpen[3][3] = {
                       { 0,    -1,      0},
                       {-1,     5,     -1},
                       { 0,    -1,      0}
                             };
///


/* Kernel de Suavizado, Smooth.*//*ConvKernelSmooth[1][1] = Mod;*/ /* Default de Mod=0; */
int ConvKernelSmooth[9] = {
                               1,   1,  1,
                               1,   0,  1,
                               1,   1,  1
                             };
///




/* Kernel de efecto MEANRENOVAL, MEANRENOVAL.*//*ConvKernelMEANRENOVAL[1][1] = Mod;*/ /* Default de Mod=9; */
int ConvKernelMEANRENOVAL[9] = {
                              -1,  -1,  -1,
                              -1,   9,  -1,
                              -1,  -1,  -1
                             };
///



/* Kernel de deteccion de orillas, EdgeDetectionQuick. */
int ConvKernelEdgeDetectionQuick[3][3] = {
                             { -1,  -1, -1},
                             {  0,   0,  0},
                             {  1,   1,  1}
                             };
///




/* Kernels de Deteccion de orillas Metodo Sobel.*/
/* Kernel Horizontal.*/
int ConvKernelSobelH[3][3] = {
                             {  1,   0, -1},
                             {  2,   0, -2},
                             {  1,   0, -1}
                             };
/* Kernel Vertical.*/
int ConvKernelSobelV[3][3] = {
                             {  1,   2,  1},
                             {  0,   0,  0},
                             { -1,  -2, -1}
                             };
///








/* Kernels de Deteccion de orillas Metodo Prewitt.*/
/* Kernel Horizontal.*/
int ConvKernelPrewittH[3][3] = {
                             {  1,   0, -1},
                             {  1,   0, -1},
                             {  1,   0, -1}
                             };
/* Kernel Vertical.*/
int ConvKernelPrewittV[3][3] = {
                             {  1,   1,  1},
                             {  0,   0,  0},
                             {  -1,  -1, -1}
                             };
///







/* Kernels de Deteccion de orillas Metodo Kirsh.*/
/* Kernel Horizontal.*/
int ConvKernelKirshH[3][3] = {
                             {  5,  -3, -3},
                             {  3,   0, -3},
                             {  5,  -3, -3}
                             };
/* Kernel Vertical.*/
int ConvKernelKirshV[3][3] = {
                             {  5,   5,  5},
                             { -3,   0, -3},
                             { -3,  -3, -3}
                             };
///






