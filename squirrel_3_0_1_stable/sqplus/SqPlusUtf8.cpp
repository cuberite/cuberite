
///////////////////////////////////////////////////////////////////////
// Simple conversion routines, to, from UTF8 and full Unicode character
// (using int). 
// 

// Only when needed 
#if !defined(SQPLUS_USE_LATIN1) || SQPLUS_USE_LATIN1==0

static char g_utf8_length[256];
static int g_did_init_length; 

void sqplus_init_utf8_lengths() {
    // Fill in lengths in array above
    for( int lb=0; lb<256; lb++ ){
        int l = -1;
        if( !(lb&0x80) ) l=1;
        else if( (lb&0xE0)==0xC0 ) l=2;
        else if( (lb&0xF0)==0xE0 ) l=3;
        else if( (lb&0xF8)==0xF0 ) l=4;
        else if( (lb&0xFC)==0xF8 ) l=5;
        else if( (lb&0xFE)==0xFC ) l=6;
        g_utf8_length[lb] = l;
    }
    g_did_init_length = 1;
}

// Length of a UTF8 encoded Unicode character
int sqplus_utf8_len(int lead_byte){
    if( !(lead_byte&0x80) ) return 1;  // Special case, make faster
    if( !g_did_init_length )
        sqplus_init_utf8_lengths();
        
    return g_utf8_length[(unsigned char)lead_byte];
}

int sqplus_utf8_len_first(const char* pc){
    int lb = *(unsigned char*)pc;
    if( !(lb&0x80) ) return 1;  // Special case, make faster
    if( !g_did_init_length )
        sqplus_init_utf8_lengths();

    int l = g_utf8_length[(unsigned char)lb];
    if( l>0 ) return l;
    
    // Invalid UTF8 lead byte. Look for next valid character. 
    const char *pc1 = pc+1;
    while( ((*pc1)&0xC0)==0x80 ) 
        pc1++;
    return int(pc1 - pc);
}


// Length of a UTF8 encoded Unicode string (number of Unicode characters)
int sqplus_utf8_strlen(const char *str) {
    if( !str ) return 0;
    int l, tl=0;
    while( *str ){
        l = sqplus_utf8_len_first(str);
        str += l;
        tl++;
    }
    return tl;
}

// Convert one UTF8 encoded character to Unicode point
int sqplus_utf8_to_wchar(int *result, const char *string){
    int res=-1;
    
    // Assume argument pointers to be OK
    unsigned char ch = *string;
    int l = sqplus_utf8_len(ch);
    
    if( l<1  ) return -1;
    int wc = l>1 ? (ch&(0x7F>>l)) : ch;
    while( l>1 ){
        wc = (wc<<6) + (*++string & 0x3F);
        l--;
    }
    *result = wc;
    
    return 0;
}

// Convert one Unicode point to UTF8 encoded version.
// Checks if output fits in 1/4/6 bytes buffer.
int sqplus_wchar_to_utf8(char *s, int wc, int size){
    if( size<1 ) return -1;
    if( (unsigned int)wc>=0x80000000 ) return -2;
    
    // Single byte case
    if( wc<0x80 ){
        *s = (char)wc;
        //*s = (char)wc&0x7F;
        return 1;
    }
    if( size<4 ) return -3;

    // Two or more UTF8 bytes
    int p = 1;  // Index of last UTF8 byte
    if( wc>0x7FF ){ // 11 bits
        // Three or more UTF8 bytes
        p++; // p>=2
        if( wc>0xFFFF ){    // 16 bits
            // Four or more UTF8 bytes
            p++; // p>=3
            if( wc>0x1FFFFF ){  // 21 bits
                // Five or more UTF8 bytes
                if( size<6 ) return -3;
                p++; // p>=4 UTF8 bytes
                if( wc>0x3FFFFFF ){ // 26 bits
                    // Six UTF8 bytes
                    p++; // p>=5 
                    if( (unsigned int)wc>(unsigned int)0x7FFFFFF ){  // 31 bits
						// Would need 7 UTF8 bytes. Not supported.
						return -10;
                    }
                    s[p-4] = 0x80 | ((wc>>24)&0x3F);    // Bit 24..29
                }
                s[p-3] = 0x80 | ((wc>>18)&0x3F);    // Bit 18..23
            }
            s[p-2] = 0x80 | ((wc>>12)&0x3F);    // Bit 12..17
        }
        s[p-1] = 0x80 | ((wc>>6)&0x3F);    // Bit 6..11
    }
    s[p] = 0x80 | (wc&0x3F);    // Bit 0..5
    s[0] = (0xFC << (5-p)) | (wc>>(p*6));
    
    return p+1;
}

#endif // #if !defined(SQPLUS_USE_LATIN1) || SQPLUS_USE_LATIN1==0

