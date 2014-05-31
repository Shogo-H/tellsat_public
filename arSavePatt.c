int arSavePatt( ARUint8 *image, ARMarkerInfo *marker_info, char *filename )
{
     FILE      *fp;
     ARUint8   ext_pat[4][AR_PATT_SIZE_Y][AR_PATT_SIZE_X][3];
     int       vertex[4];
     int       i, j, k, x, y;
 
         // Match supplied info against previously recognised marker.
     for( i = 0; i < wmarker_num; i++ ) {
         if( marker_info->area   == marker_info2[i].area
          && marker_info->pos[0] == marker_info2[i].pos[0]
          && marker_info->pos[1] == marker_info2[i].pos[1] ) break;
     }
     if( i == wmarker_num ) return -1;
 
     for( j = 0; j < 4; j++ ) {
         for( k = 0; k < 4; k++ ) {
             vertex[k] = marker_info2[i].vertex[(k+j+2)%4];
         }
         if ( arGetPatt( image, marker_info2[i].x_coord, marker_info2[i].y_coord, vertex, ext_pat[j] ) < 0 )
                                         return -1;
     }
 
     fp = fopen( filename, "w" );
     if( fp == NULL ) return -1;
 
         // Write out in order AR_PATT_SIZE_X columns x AR_PATT_SIZE_Y rows x 3 colours x 4 orientations.
     for( i = 0; i < 4; i++ ) {
         for( j = 0; j < 3; j++ ) {
             for( y = 0; y < AR_PATT_SIZE_Y; y++ ) {
                 for( x = 0; x < AR_PATT_SIZE_X; x++ ) {
                     fprintf( fp, "%4d", ext_pat[i][y][x][j] );
                 }
                 fprintf(fp, "\n");
             }
         }
         fprintf(fp, "\n");
     }
 
     fclose( fp );
 
     return 0;
 }
