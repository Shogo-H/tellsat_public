#if ( arTemplateMatchingMode == AR_TEMPLATE_MATCHING_COLOR )
 int arGetPatt( ARUint8 *image, int *x_coord, int *y_coord, int *vertex, ARUint8 ext_pat[AR_PATT_SIZE_Y][AR_PATT_SIZE_X][3], int *ext_pat_ave )
 {
         ARUint32  ext_pat2[AR_PATT_SIZE_Y][AR_PATT_SIZE_X][3];
 #else
 int arGetPatt( ARUint8 *image, int *x_coord, int *y_coord, int *vertex, ARUint16 ext_pat[AR_PATT_SIZE_Y][AR_PATT_SIZE_X], int *ext_pat_ave )
 {
         ARUint32  ext_pat2[AR_PATT_SIZE_Y][AR_PATT_SIZE_X];
 #endif
         ARVAL    world[4][2];
         ARVAL    local[4][2];
         ARVAL    para[3][3];
         ARVAL    d, dc, xw, yw, xwc, ywc;
         int       xc, yc;
         int       xdivshift, ydivshift;
         int       xdiv2shift, ydiv2shift;
         int       lx1, lx2, ly1, ly2;
         int       ext_pat2_x_index, ext_pat2_y_index;
         int       image_index;
         int       i, j;
         int       ave;
 
         world[0][0] = IntToARVAL( 100 );
         world[0][1] = IntToARVAL( 100 );
         world[1][0] = IntToARVAL( 100 + 10 );
         world[1][1] = IntToARVAL( 100 );
         world[2][0] = IntToARVAL( 100 + 10 );
         world[2][1] = IntToARVAL( 100 + 10 );
         world[3][0] = IntToARVAL( 100 );
         world[3][1] = IntToARVAL( 100 + 10 );
         for( i = 0; i < 4; i++ ) {
                 local[i][0] = IntToARVAL( x_coord[vertex[i]] );
                 local[i][1] = IntToARVAL( y_coord[vertex[i]] );
         }
         get_cpara( world, local, para );
 
         lx1 = matNorm( x_coord[vertex[0]] - x_coord[vertex[1]],  y_coord[vertex[0]] - y_coord[vertex[1]] );
         lx2 = matNorm( x_coord[vertex[2]] - x_coord[vertex[3]],  y_coord[vertex[2]] - y_coord[vertex[3]] );
         ly1 = matNorm( x_coord[vertex[1]] - x_coord[vertex[2]],  y_coord[vertex[1]] - y_coord[vertex[2]] );
         ly2 = matNorm( x_coord[vertex[3]] - x_coord[vertex[0]],  y_coord[vertex[3]] - y_coord[vertex[0]] );
         if( lx2 > lx1 ) lx1 = lx2;
         if( ly2 > ly1 ) ly1 = ly2;
         xdiv2shift = AR_PATT_SIZE_X_SHIFT;
         ydiv2shift = AR_PATT_SIZE_Y_SHIFT;
         if( arImageProcMode == AR_IMAGE_PROC_IN_FULL ) {
                 while( 1<<(xdiv2shift*2) < lx1  &&  xdiv2shift < AR_PATT_SAMPLE_NUM_SHIFT ){ xdiv2shift++; }
                 while( 1<<(ydiv2shift*2) < ly1  &&  ydiv2shift < AR_PATT_SAMPLE_NUM_SHIFT ){ ydiv2shift++; }
         }
         else {
                 while( 1<<(xdiv2shift*2+2) < lx1  &&  xdiv2shift < AR_PATT_SAMPLE_NUM_SHIFT ){ xdiv2shift++; }
                 while( 1<<(ydiv2shift*2+2) < ly1  &&  ydiv2shift < AR_PATT_SAMPLE_NUM_SHIFT ){ ydiv2shift++; }
         }
         if( xdiv2shift > AR_PATT_SAMPLE_NUM_SHIFT ){ xdiv2shift = AR_PATT_SAMPLE_NUM_SHIFT; }
         if( ydiv2shift > AR_PATT_SAMPLE_NUM_SHIFT ){ ydiv2shift = AR_PATT_SAMPLE_NUM_SHIFT; }
 
         xdivshift = xdiv2shift - AR_PATT_SIZE_X_SHIFT;
         ydivshift = ydiv2shift - AR_PATT_SIZE_Y_SHIFT;
         /*
         printf("%3d(%f), %3d(%f)\n", xdiv2, sqrt(lx1), ydiv2, sqrt(ly1));
         */
 
         put_zero( (ARUint8 *)ext_pat2, sizeof(ext_pat2) );
         yw =  DoubleToARVAL( 102.5 ) + (IntToARVAL( 5 ) >> (ydiv2shift+1)) - (IntToARVAL( 5 ) >> ydiv2shift); //102.5 + 5.0 * (j+0.5) * ydiv2_reciprocal
 
 
         for( j=0; j < 1<<ydiv2shift; j++ ) 
         {
                 yw += IntToARVAL( 5 ) >> ydiv2shift;
                 xwc = MulARVAL( para[0][1], yw ) + para[0][2];
                 ywc = MulARVAL( para[1][1], yw ) + para[1][2];
                 dc  = MulARVAL( para[2][1], yw ) + para[2][2];
                 
                 xw = DoubleToARVAL( 102.5 ) + (IntToARVAL( 5 ) >> (xdiv2shift+1)) - (IntToARVAL( 5 ) >> xdiv2shift); //102.5 + 5.0 * (i+0.5) * xdiv2_reciproc
                 
                 for( i=0; i < 1<<xdiv2shift; i++ ) 
                 {
                         xw += IntToARVAL( 5 ) >> xdiv2shift;
                         
                         d = MulARVAL( para[2][0], xw ) + dc;
                         if ( d == 0 ) 
                                 return -1;
 
                         xc = ARVALtoInt( DivARVAL( MulARVAL( para[0][0], xw ) + xwc, d ) );
                         yc = ARVALtoInt( DivARVAL( MulARVAL( para[1][0], xw ) + ywc, d ) );
 
                         if( arImageProcMode == AR_IMAGE_PROC_IN_HALF ) 
                         {
                                 // 最下位ビットを切り上げる
                                 xc = (xc+1) & ~1;
                                 yc = (yc+1) & ~1;
                         }
                         if( xc >= 0 && xc < arImXsize && yc >= 0 && yc < arImYsize ) 
                         {
                                 ext_pat2_y_index = j >> ydivshift;
                                 ext_pat2_x_index = i >> xdivshift;
                                 image_index = (yc*arImXsize+xc)*AR_PIX_SIZE_DEFAULT;
 #if ( arTemplateMatchingMode == AR_TEMPLATE_MATCHING_COLOR )
 #       if (AR_DEFAULT_PIXEL_FORMAT == AR_PIXEL_FORMAT_ARGB)
                                 ext_pat2[ext_pat2_y_index][ext_pat2_x_index][0] += image[image_index+3];
                                 ext_pat2[ext_pat2_y_index][ext_pat2_x_index][1] += image[image_index+2];
                                 ext_pat2[ext_pat2_y_index][ext_pat2_x_index][2] += image[image_index+1];
 #       elif (AR_DEFAULT_PIXEL_FORMAT == AR_PIXEL_FORMAT_ABGR)
                                 ext_pat2[ext_pat2_y_index][ext_pat2_x_index][0] += image[image_index+1];
                                 ext_pat2[ext_pat2_y_index][ext_pat2_x_index][1] += image[image_index+2];
                                 ext_pat2[ext_pat2_y_index][ext_pat2_x_index][2] += image[image_index+3];
 #       elif (AR_DEFAULT_PIXEL_FORMAT == AR_PIXEL_FORMAT_BGRA)
                                 ext_pat2[ext_pat2_y_index][ext_pat2_x_index][0] += image[image_index+0];
                                 ext_pat2[ext_pat2_y_index][ext_pat2_x_index][1] += image[image_index+1];
                                 ext_pat2[ext_pat2_y_index][ext_pat2_x_index][2] += image[image_index+2];
 #       elif (AR_DEFAULT_PIXEL_FORMAT == AR_PIXEL_FORMAT_BGR)
                                 ext_pat2[ext_pat2_y_index][ext_pat2_x_index][0] += image[image_index+0];
                                 ext_pat2[ext_pat2_y_index][ext_pat2_x_index][1] += image[image_index+1];
                                 ext_pat2[ext_pat2_y_index][ext_pat2_x_index][2] += image[image_index+2];
 #       elif (AR_DEFAULT_PIXEL_FORMAT == AR_PIXEL_FORMAT_RGBA)
                                 ext_pat2[ext_pat2_y_index][ext_pat2_x_index][0] += image[image_index+2];
                                 ext_pat2[ext_pat2_y_index][ext_pat2_x_index][1] += image[image_index+1];
                                 ext_pat2[ext_pat2_y_index][ext_pat2_x_index][2] += image[image_index+0];
 #       elif (AR_DEFAULT_PIXEL_FORMAT == AR_PIXEL_FORMAT_RGB)
                                 ext_pat2[ext_pat2_y_index][ext_pat2_x_index][0] += image[image_index+2];
                                 ext_pat2[ext_pat2_y_index][ext_pat2_x_index][1] += image[image_index+1];
                                 ext_pat2[ext_pat2_y_index][ext_pat2_x_index][2] += image[image_index+0];
 #       elif (AR_DEFAULT_PIXEL_FORMAT == AR_PIXEL_FORMAT_MONO)
                                 ext_pat2[ext_pat2_y_index][ext_pat2_x_index][0] += image[image_index];
                                 ext_pat2[ext_pat2_y_index][ext_pat2_x_index][1] += image[image_index];
                                 ext_pat2[ext_pat2_y_index][ext_pat2_x_index][2] += image[image_index];
 #       elif (AR_DEFAULT_PIXEL_FORMAT == AR_PIXEL_FORMAT_2vuy)
                                 ext_pat2[ext_pat2_y_index][ext_pat2_x_index][0] += image[image_index+1];
                                 ext_pat2[ext_pat2_y_index][ext_pat2_x_index][1] += image[image_index+1];
                                 ext_pat2[ext_pat2_y_index][ext_pat2_x_index][2] += image[image_index+1];
 #       elif (AR_DEFAULT_PIXEL_FORMAT == AR_PIXEL_FORMAT_yuvs)
                                 ext_pat2[ext_pat2_y_index][ext_pat2_x_index][0] += image[image_index+0];
                                 ext_pat2[ext_pat2_y_index][ext_pat2_x_index][1] += image[image_index+0];
                                 ext_pat2[ext_pat2_y_index][ext_pat2_x_index][2] += image[image_index+0];
 #       elif (AR_DEFAULT_PIXEL_FORMAT == AR_PIXEL_FORMAT_RGB565)
                                 {
                                         int r,g,b;
                                         unsigned short pixel = *(unsigned short*)&image[image_index];
                                         getRGBfromRGB565( r, g, b, pixel );
                                         ext_pat2[ext_pat2_y_index][ext_pat2_x_index][0] += b;
                                         ext_pat2[ext_pat2_y_index][ext_pat2_x_index][1] += g;
                                         ext_pat2[ext_pat2_y_index][ext_pat2_x_index][2] += r;
                                 }
 #       else
 #         error Unknown default pixel format defined in config.h
 #       endif
 #else
 #       if (AR_DEFAULT_PIXEL_FORMAT == AR_PIXEL_FORMAT_ARGB) || (AR_DEFAULT_PIXEL_FORMAT == AR_PIXEL_FORMAT_ABGR)
                                 ext_pat2[ext_pat2_y_index][ext_pat2_x_index] += image[image_index+1] + image[image_index+2] + image[image_index+3];
 #       elif (AR_DEFAULT_PIXEL_FORMAT == AR_PIXEL_FORMAT_BGRA) || (AR_DEFAULT_PIXEL_FORMAT == AR_PIXEL_FORMAT_BGR) || (AR_DEFAULT_PIXEL_FORMAT == AR_PIXEL_FORMAT_RGB) || (AR_DEFAULT_PIXEL_FORMAT == AR_PIXEL_FORMAT_RGBA)
                                 ext_pat2[ext_pat2_y_index][ext_pat2_x_index] += image[image_index+0] + image[image_index+1] + image[image_index+2];
 #       elif (AR_DEFAULT_PIXEL_FORMAT == AR_PIXEL_FORMAT_MONO)
                                 ext_pat2[ext_pat2_y_index][ext_pat2_x_index] += image[image_index]*3;
 #       elif (AR_DEFAULT_PIXEL_FORMAT == AR_PIXEL_FORMAT_2vuy)
                                 ext_pat2[ext_pat2_y_index][ext_pat2_x_index] += image[image_index+1]*3;
 #       elif (AR_DEFAULT_PIXEL_FORMAT == AR_PIXEL_FORMAT_yuvs)
                                 ext_pat2[ext_pat2_y_index][ext_pat2_x_index] += image[image_index+0]*3;
 #       elif (AR_DEFAULT_PIXEL_FORMAT == AR_PIXEL_FORMAT_RGB565)
                                 {
                                         int r,g,b;
                                         unsigned short pixel = *(unsigned short*)&image[image_index];
                                         getRGBfromRGB565( r, g, b, pixel );
                                         ext_pat2[ext_pat2_y_index][ext_pat2_x_index] += b+g+r;
                                 }
 #       else
 #         error Unknown default pixel format defined in config.h
 #       endif
 #endif
                         }
                 }
         }
 
         xdivshift += ydivshift;
         ave = 0;
         for( j = 0; j < AR_PATT_SIZE_Y; j++ ) {
                 for( i = 0; i < AR_PATT_SIZE_X; i++ ) {                         // PRL 2006-06-08.
 #if ( arTemplateMatchingMode == AR_TEMPLATE_MATCHING_COLOR )
                         ext_pat[j][i][0] = 255 - (ARUint8)(ext_pat2[j][i][0] >> xdivshift);
                         ext_pat[j][i][1] = 255 - (ARUint8)(ext_pat2[j][i][1] >> xdivshift);
                         ext_pat[j][i][2] = 255 - (ARUint8)(ext_pat2[j][i][2] >> xdivshift);
                         ave += ext_pat[j][i][0] + ext_pat[j][i][1] + ext_pat[j][i][2];
 #else
                         ext_pat[j][i] = 255*3 - (ARUint16)(ext_pat2[j][i] >> xdivshift);
                         ave += ext_pat[j][i];
 #endif
                 }
         }
         ave >>= AR_PATT_SIZE_X_SHIFT + AR_PATT_SIZE_Y_SHIFT;
         *ext_pat_ave = ave;
 
         return(0);
 }
