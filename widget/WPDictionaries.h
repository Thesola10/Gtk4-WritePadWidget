#ifndef __WPDICTIONARIES_H
#define __WPDICTIONARIES_H

#include <langid.h>

#define stringify$(s) #s
#define $PREFIX stringify$(CMAKE_INSTALL_PREFIX) "/share/WritePad/Dictionaries/"

const char *WP_DICT_DEFAULTS[LANGUAGE_SIZE] = {
    "",                         /* LANGUAGE_NONE        */
    $PREFIX "English.dct",      /* LANGUAGE_ENGLISH     */
    $PREFIX "French.dct",       /* LANGUAGE_FRENCH      */
    $PREFIX "German.dct",       /* LANGUAGE_GERMAN      */
    $PREFIX "Spanish.dct",      /* LANGUAGE_SPANISH     */
    $PREFIX "Italian.dct",      /* LANGUAGE_ITALIAN     */
    $PREFIX "Swedish.dct",      /* LANGUAGE_SWEDISH     */
    $PREFIX "Norwegian.dct",    /* LANGUAGE_NORWEGIAN   */
    $PREFIX "Dutch.dct",        /* LANGUAGE_DUTCH       */
    $PREFIX "Danish.dct",       /* LANGUAGE_DANISH      */
    $PREFIX "Portuguese.dct",   /* LANGUAGE_PORTUGUESE  */
    $PREFIX "Brazilian.dct",    /* LANGUAGE_PORTUGUESEB */
    $PREFIX "MedicalUS.dct",    /* LANGUAGE_MEDICAL     */
    $PREFIX "Finnish.dct",      /* LANGUAGE_FINNISH     */
    $PREFIX "Indonesian.dct",   /* LANGUAGE_INDONESIAN  */
    $PREFIX "EnglishUK.dct",    /* LANGUAGE_ENGLISHUK   */
};

#endif //__WPDICTIONARIES_H
