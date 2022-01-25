//
//  structure.h
//  CheckTranslated
//
//  Created by Koichi MATSUMOTO on 2019/05/26.
//  Copyright © 2019 Koichi MATSUMOTO. All rights reserved.
//

#ifndef structure_h
#define structure_h

typedef struct {
    char * name;
    char * key;
    char * ref_name;   //
    char * ref_key;    //
    char * index;      //
    char * value;
    char * translation;
} LocaleTSV;

typedef struct {
    char * line_num;    // line number
    char * id;          //
    char * key;       //
    char * index;         //
    char * value;
    char * translation;
    
} TranslatedTSV;

#define TAB         '\t'
#define SPC         ' '
#define CRLF        '\n'
#define BUFFER_SIZE (8192 * 4)

#endif /* structure_h */
