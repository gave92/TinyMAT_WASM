/*
    Copyright (c) 2008-2015 Jan W. Krieger (<jan@jkrieger.de>, <j.krieger@dkfz.de>), German Cancer Research Center (DKFZ) & IWR, University of Heidelberg

    last modification: $LastChangedDate: 2015-07-07 12:07:58 +0200 (Di, 07 Jul 2015) $  (revision $Rev: 4005 $)

    This software is free software: you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License (LGPL) as published by
    the Free Software Foundation, either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.


*/
#include <math.h>
#include <float.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

//#include <iostream>

#include "tinymatwriter.h"

#ifndef __WINDOWS__
# if defined(WIN32) || defined(WIN64) || defined(_MSC_VER) || defined(_WIN32)
#  define __WINDOWS__
# endif
#endif

#ifndef __LINUX__
# if defined(linux)
#  define __LINUX__
# endif
#endif
#ifdef TINYMAT_USES_QVARIANT
//#  include <QDebug>
#  include <QPoint>
#  include <QSize>
#endif

#define TINYMAT_ORDER_UNKNOWN 0
#define TINYMAT_ORDER_BIGENDIAN 1
#define TINYMAT_ORDER_LITTLEENDIAN 2


#if defined(__GNUC__) || defined(__GNUG__)
#define TINYMAT_inlineattrib  inline
// use this for no-inline:
//#define TINYMAT_inlineattrib  __attribute__((noinline))
#else
#define TINYMAT_noinline inline
#endif



/*! \brief this struct represents a mat file
    \ingroup TinyMATwriter
    \internal
 */
struct TinyMATWriterFile {
    /* \brief the libc file handle */
    FILE* file;

    /* \brief specifies the byte order of the system (and the written file!) */
    uint8_t byteorder;
};


int TinyMATWriter_fOK(const TinyMATWriterFile* mat)  {
    return (mat && (mat->file!=NULL));
}


/*! \brief determines the byte order of the system
    \ingroup tinymatwriter
    \internal

    \return TINYMAT_ORDER_BIGENDIAN or TINYMAT_ORDER_LITTLEENDIAN, or TINYMAT_ORDER_UNKNOWN if the byte order cannot be determined
 */
 TINYMAT_inlineattrib static int TinyMAT_get_byteorder()
{
        union {
                long l;
                char c[4];
        } test;
        test.l = 1;
        if( test.c[3] && !test.c[2] && !test.c[1] && !test.c[0] )
                return TINYMAT_ORDER_BIGENDIAN;

        if( !test.c[3] && !test.c[2] && !test.c[1] && test.c[0] )
                return TINYMAT_ORDER_LITTLEENDIAN;

        return TINYMAT_ORDER_UNKNOWN;
}


 TINYMAT_inlineattrib static int TinyMAT_fclose(TinyMATWriterFile* file) {
     //std::cout<<"TinyMAT_fclose()\n";
     //std::cout.flush();
     if (!file || !file->file) return 0;
     int ret= fclose(file->file);
     free(file);
     return ret;
 }

 TINYMAT_inlineattrib static TinyMATWriterFile* TinyMAT_fopen(const char* filename) {
     //std::cout<<"TinyMAT_fopen()\n";
     //std::cout.flush();
     TinyMATWriterFile* mat=(TinyMATWriterFile*)malloc(sizeof(TinyMATWriterFile));

     mat->file=fopen(filename, "wb");
     mat->byteorder=TinyMAT_get_byteorder();
     return mat;
 }

  TINYMAT_inlineattrib static long TinyMAT_ftell(TinyMATWriterFile* file) {
     //std::cout<<"TinyMAT_ftell()\n";
     //std::cout.flush();
     if (!file || !file->file) return 0;
     return ftell(file->file);
 }
 TINYMAT_inlineattrib static int TinyMAT_fseek(TinyMATWriterFile* file, long offset, int base) {
     //std::cout<<"TinyMAT_fseek()\n";
     //std::cout.flush();
     if (!file || !file->file) return 0;
     return fseek(file->file, offset, base);
 }


 TINYMAT_inlineattrib static int TinyMAT_fwrite(const void* data, uint32_t size, uint32_t count, TinyMATWriterFile* file)
{
     //std::cout<<"TinyMAT_fwrite()\n";
     if (!file || !file->file || !data || size*count<=0) return 0;
     int res=fwrite(data, 1, size*count, file->file);
     return res;
}




TINYMAT_inlineattrib static void TinyMAT_write8(TinyMATWriterFile* filen, uint8_t data) {
    if (!filen) return;
    TinyMAT_fwrite(&data, 1, 1, filen);
}
TINYMAT_inlineattrib static void TinyMAT_write8(TinyMATWriterFile* filen, int8_t data) {
    if (!filen) return;
    TinyMAT_fwrite(&data, 1, 1, filen);
}
TINYMAT_inlineattrib static void TinyMAT_write16(TinyMATWriterFile* filen, uint16_t data) {
    if (!filen) return;
    TinyMAT_fwrite(&data, 2, 1, filen);
}
TINYMAT_inlineattrib static void TinyMAT_write16(TinyMATWriterFile* filen, int16_t data) {
    if (!filen) return;
    TinyMAT_fwrite(&data, 2, 1, filen);
}
TINYMAT_inlineattrib static void TinyMAT_write32(TinyMATWriterFile* filen, uint32_t data) {
    if (!filen) return;
    TinyMAT_fwrite(&data, 4, 1, filen);

}
TINYMAT_inlineattrib static void TinyMAT_write32(TinyMATWriterFile* filen, int32_t data) {
    if (!filen) return;
    TinyMAT_fwrite(&data, 4, 1, filen);

}
TINYMAT_inlineattrib static void TinyMAT_write64(TinyMATWriterFile* filen, uint64_t data) {
    if (!filen) return;
    TinyMAT_fwrite(&data, 8, 1, filen);
}
TINYMAT_inlineattrib static void TinyMAT_write64(TinyMATWriterFile* filen, int64_t data) {
    if (!filen) return;
    TinyMAT_fwrite(&data, 8, 1, filen);
}
TINYMAT_inlineattrib static void TinyMAT_write64d(TinyMATWriterFile* filen, double data) {
    if (!filen) return;
    TinyMAT_fwrite(&data, 8, 1, filen);
}
TINYMAT_inlineattrib static void TinyMAT_write32f(TinyMATWriterFile* filen, float data) {
    if (!filen) return;
    TinyMAT_fwrite(&data, 4, 1, filen);
}


#define TINYMAT_miINT8 1
#define TINYMAT_miUINT8 2
#define TINYMAT_miINT16 3
#define TINYMAT_miUINT16 4
#define TINYMAT_miINT32 5
#define TINYMAT_miUINT32 6
#define TINYMAT_miSINGLE 7
#define TINYMAT_miDOUBLE 9
#define TINYMAT_miINT64 12
#define TINYMAT_miUINT64 13
#define TINYMAT_miMATRIX 14
#define TINYMAT_miCOMPRESSED 15
#define TINYMAT_miUTF8 16
#define TINYMAT_miUTF16 17
#define TINYMAT_miUTF32 18





TINYMAT_inlineattrib static void TinyMAT_writeDatElement_u64(TinyMATWriterFile* mat, uint64_t data) {
	TinyMAT_write32(mat, (uint32_t)TINYMAT_miUINT64);
	TinyMAT_write32(mat, (uint32_t)8);
	TinyMAT_write64(mat, data);
}

TINYMAT_inlineattrib static void TinyMAT_writeDatElement_i64(TinyMATWriterFile* mat, int64_t data) {
	TinyMAT_write32(mat, (uint32_t)TINYMAT_miINT64);
	TinyMAT_write32(mat, (uint32_t)8);
	TinyMAT_write64(mat, data);
}

TINYMAT_inlineattrib static void TinyMAT_writeDatElement_u32(TinyMATWriterFile* mat, uint32_t data) {
	TinyMAT_write32(mat, (uint32_t)TINYMAT_miUINT32);
	TinyMAT_write32(mat, (uint32_t)4);
	TinyMAT_write32(mat, data);
	TinyMAT_write32(mat, (uint32_t)0); // write padding
}

TINYMAT_inlineattrib static void TinyMAT_writeDatElement_i32(TinyMATWriterFile* mat, int32_t data) {
	TinyMAT_write32(mat, (uint32_t)TINYMAT_miINT32);
	TinyMAT_write32(mat, (uint32_t)4);
	TinyMAT_write32(mat, data);
	TinyMAT_write32(mat, (uint32_t)0); // write padding
}

TINYMAT_inlineattrib static void TinyMAT_writeDatElement_u16(TinyMATWriterFile* mat, uint16_t data) {
	TinyMAT_write32(mat, (uint32_t)TINYMAT_miUINT16);
	TinyMAT_write32(mat, (uint32_t)2);
	TinyMAT_write16(mat, data);
	TinyMAT_write16(mat, (uint16_t)0); // write padding
	TinyMAT_write32(mat, (uint32_t)0);
}

TINYMAT_inlineattrib static void TinyMAT_writeDatElement_i16(TinyMATWriterFile* mat, int16_t data) {
	TinyMAT_write32(mat, (uint32_t)TINYMAT_miINT16);
	TinyMAT_write32(mat, (uint32_t)2);
	TinyMAT_write16(mat, data);
	TinyMAT_write16(mat, (uint16_t)0); // write padding
	TinyMAT_write32(mat, (uint32_t)0);
}

TINYMAT_inlineattrib static void TinyMAT_writeDatElement_dbl(TinyMATWriterFile* mat, double data) {
	TinyMAT_write32(mat, (uint32_t)TINYMAT_miDOUBLE);
	TinyMAT_write32(mat, (uint32_t)8);
	TinyMAT_write64d(mat, data);
	// no padding required
}
TINYMAT_inlineattrib static void TinyMAT_writeDatElement_dbla(TinyMATWriterFile* mat, const double* data, uint32_t items) {
    long spos=TinyMAT_ftell(mat);
	TinyMAT_write32(mat, (uint32_t)TINYMAT_miDOUBLE);
    if (!data) items=0;
    TinyMAT_write32(mat, items*8);
    if (items>0 && data){
        TinyMAT_fwrite(data, 8, items, mat);
    }

    // check for correct pos in file!
    if (TinyMAT_ftell(mat)!=(int64_t)(spos+8+items*8)) {
        TinyMAT_fseek(mat, spos+(int64_t)(8+items*8),SEEK_SET);
    }
}
TINYMAT_inlineattrib static void TinyMAT_writeDatElement_dbla_rowmajor(TinyMATWriterFile* mat, const double* data, size_t rows, size_t cols) {
	TinyMAT_write32(mat, (uint32_t)TINYMAT_miDOUBLE);
	TinyMAT_write32(mat, (uint32_t)(rows*cols*8));
    if (rows*cols>0) {
        for (size_t c=0; c<cols; c++) {
            for (size_t r=0; r<rows; r++) {
                TinyMAT_fwrite(&(data[r*cols+c]), 8, 1, mat);
            }
        }
    }
	// no padding required
}
TINYMAT_inlineattrib static void TinyMAT_writeDatElement_u32a(TinyMATWriterFile* mat, const uint32_t* data, size_t items) {
	TinyMAT_write32(mat, (uint32_t)TINYMAT_miUINT32);
	TinyMAT_write32(mat, (uint32_t)(items*4));
    if (items>0) {
        TinyMAT_fwrite(data, 4, items, mat);
        // write padding
        if (items%2==1) TinyMAT_write32(mat, (uint32_t)0);
    }
}

TINYMAT_inlineattrib static void TinyMAT_writeDatElement_stringas8bit(TinyMATWriterFile* mat, const char* data, uint32_t slen) {
    //long spos=ftell(mat);
    size_t pad=(slen)%8;
    uint32_t cla=TINYMAT_miINT8;
    TinyMAT_write32(mat, cla);
    TinyMAT_write32(mat, slen);
    //if (slen>0) std::cout<<"TinyMAT_writeDatElement_stringas8bit() str="<<(char*)tmp<<"  slen="<<slen<<"  pad="<<pad<<"  spos="<<spos<<"\n";
    if (slen>0 && data) {
        TinyMAT_fwrite(data, 1, slen, mat);
        // write padding
        size_t pc=0;
        if (pad>0) {
            for (size_t i=pad; i<8; i++) {
                TinyMAT_write8(mat, (uint8_t)0);
                pc++;
                //std::cout<<"   writePAD "<<i<<"/"<<pad<<" "<<ftell(mat)<<"\n";
            }

        }
//        if (TinyMAT_ftell(mat)!=long(spos+8+slen+pc)) {
//            TinyMAT_fseek(mat, spos+8+slen+pc,SEEK_SET);
//            std::cout<<"   seek"<< spos+8+slen+pc<<" "<<slen<<" "<<pc<<"\n";
//        }

    } else {
//        if (TinyMAT_ftell(mat)!=spos+8) {
//            TinyMAT_fseek(mat, spos+8,SEEK_SET);
//            std::cout<<"   seek"<< spos+8<<"\n";
//        }

    }
    //if (data) free(data);

    //std::cout<<"  TinyMAT_writeDatElement_stringas8bit() slen="<<slen<<"  pad="<<pad<<"  spos="<<spos<<"  ftell="<<ftell(mat)<<"\n";

}

TINYMAT_inlineattrib static void TinyMAT_writeDatElement_string(TinyMATWriterFile* mat, const char* data, uint32_t slen) {
    //long spos=ftell(mat);
    size_t pad=(2*slen)%8;
    int16_t* tmp=NULL;
    if (slen>0 && data) {
        tmp=(int16_t*)malloc(slen*2);
        for (uint32_t i=0; i<slen; i++) {
            tmp[i]=data[i];
        }
    }
    uint32_t cla=TINYMAT_miUINT16;
    TinyMAT_write32(mat, cla);
    TinyMAT_write32(mat, slen*2);
    //if (slen>0) std::cout<<"TinyMAT_writeDatElement_string() slen="<<slen<<"  pad="<<pad<<"  spos="<<spos<<"\n";
    if (slen>0 && tmp) {
        TinyMAT_fwrite(tmp, 2, slen, mat);
        // write padding
        size_t pc=0;
        if (pad>0) {
            for (size_t i=pad; i<8; i++) {
                TinyMAT_write8(mat, (uint8_t)0);
                pc++;
                //std::cout<<"   writePAD "<<i<<"/"<<pad<<" "<<ftell(mat)<<"\n";
            }

        }
//        if (TinyMAT_ftell(mat)!=long(spos+8+slen*2+pc)) {
//           TinyMAT_fseek(mat, spos+8+slen*2+pc,SEEK_SET);
//            std::cout<<"   seek"<< spos+8+slen*2+pc<<" "<<slen*2<<" "<<pc<<"\n";
//        }

    } else {
//        if (TinyMAT_ftell(mat)!=spos+8) {
//           TinyMAT_fseek(mat, spos+8,SEEK_SET);
//            std::cout<<"   seek"<< spos+8<<"\n";
//        }

    }
    if (tmp) free(tmp);

    //std::cout<<"  TinyMAT_writeDatElement_string() slen="<<slen<<"  pad="<<pad<<"  spos="<<spos<<"  ftell="<<ftell(mat)<<"\n";

}

TINYMAT_inlineattrib static void TinyMAT_writeDatElement_stringas8bit(TinyMATWriterFile* mat, const char* data) {
    TinyMAT_writeDatElement_stringas8bit(mat, data, strlen(data));
}
TINYMAT_inlineattrib static void TinyMAT_writeDatElement_string(TinyMATWriterFile* mat, const char* data) {
    TinyMAT_writeDatElement_string(mat, data, strlen(data));
}

TINYMAT_inlineattrib static size_t TinyMAT_DatElement_realstringlen8bit(const char* data) {
    if (!data) return 0;
    size_t slen=strlen(data);
    if (slen==0) return 0;
    size_t pad=slen%8;
    if (pad>0) {
        slen=slen+(8-pad);
    }
    return slen;
}
TINYMAT_inlineattrib static size_t TinyMAT_DatElement_realstringlen16bit(const char* data) {
    if (!data) return 0;
    size_t slen=strlen(data)*2;
    if (slen==0) return 0;
    size_t pad=slen%8;
    if (pad>0) {
        slen=slen+(8-pad);
    }
    return slen;
}







TinyMATWriterFile* TinyMATWriter_open(const char* filename, const char* description) {
    TinyMATWriterFile* mat=TinyMAT_fopen(filename);

    if (TinyMATWriter_fOK(mat)) {



		// setup and write Description field (116 bytes)
        char stdmsg[512];
        for (int i=0; i<512; i++) stdmsg[i]='\0';
        time_t rawtime;
        ::time(&rawtime);
        struct tm * timeinfo = ::gmtime(&rawtime);
        sprintf(stdmsg,"MATLAB 5.0 MAT-file, written by TinyMAT, %d-%02d-%02d %02d:%02d:%02d UTC", 1900+timeinfo->tm_year, timeinfo->tm_mon+1, timeinfo->tm_mday, timeinfo->tm_hour, timeinfo->tm_min, timeinfo->tm_sec);
        size_t slstd=strlen(stdmsg);
        char desc[116];
		size_t sl=0;
		if (description) sl=strlen(description);
		if (slstd>0) {
			for (size_t i=0; i<slstd; i++) {
				desc[i]=stdmsg[i];
			}
		}
		size_t maxv=slstd;
		if (sl>0) {
		    maxv=maxv+2+sl;
		}
		if (maxv>116) maxv=116;
		if (sl>0) {
			desc[slstd]=':';
			desc[slstd+1]=' ';
			for (size_t i=slstd+2; i<sl; i++) {
				desc[i]=description[i];
			}
		}
		if (maxv<116) {
		    desc[maxv]='\0';
			for (size_t i=maxv+1; i<116; i++) {
				desc[i]=' ';
			}
		} else {
			desc[115]='\0';
		}
        TinyMAT_fwrite(desc, 1, 116,mat);
		
		// write "Header Subsystem Data Offset Field" (not used, i.e. write 00000000)
        TinyMAT_write32(mat, (uint32_t)0x00000000);
        TinyMAT_write32(mat, (uint32_t)0x00000000);
		
		// write "Header Flag Fields"
        TinyMAT_write16(mat, (uint16_t)0x0100); // version
        TinyMAT_write8(mat, (int8_t)'I'); // endian indicator
        TinyMAT_write8(mat, (int8_t)'M');
		return mat;
    } else {
        free(mat);
        return NULL;
    }
}

#define TINYMAT_mxDOUBLE_CLASS_arrayflags 0x00000006
#define TINYMAT_mxCHAR_CLASS_CLASS_arrayflags 0x00000004
#define TINYMAT_mxCELL_CLASS_arrayflags 0x00000001

void TinyMATWriter_writeMatrix2D_colmajor(TinyMATWriterFile* mat, const char* name, const double* data_real, int32_t cols, int32_t rows) {
	uint32_t size_bytes=0;
    uint32_t arrayflags[2]={TINYMAT_mxDOUBLE_CLASS_arrayflags, 0};
	
	
	size_bytes+=16; // array flags
	size_bytes+=16; // dimensions flags
    size_bytes+=8+TinyMAT_DatElement_realstringlen8bit(name); // array name
    size_bytes+=8+8*cols*rows; // actual data
	
	// write tag header
    TinyMAT_write32(mat, (uint32_t)TINYMAT_miMATRIX);
    TinyMAT_write32(mat, size_bytes);
	
	// write arrayflags
    TinyMAT_writeDatElement_u32a(mat, arrayflags, 2);

	// write field dimensions
    TinyMAT_write32(mat, (uint32_t)TINYMAT_miINT32);
    TinyMAT_write32(mat, (uint32_t)8);
    TinyMAT_write32(mat, cols);
    TinyMAT_write32(mat, rows);
	
	// write field name
    TinyMAT_writeDatElement_stringas8bit(mat, name);

	
	// write data type
    TinyMAT_writeDatElement_dbla(mat, data_real, cols*rows);
	
}



void TinyMATWriter_writeString(TinyMATWriterFile *mat, const char *name, const char *data)
{
    TinyMATWriter_writeString(mat, name, data, strlen(data));
}

void TinyMATWriter_writeString(TinyMATWriterFile *mat, const char *name, const char *data, uint32_t slen)
{
    uint32_t size_bytes=0;
    uint32_t arrayflags[2];
    arrayflags[0]=TINYMAT_mxCHAR_CLASS_CLASS_arrayflags;
    arrayflags[1]=0;



    size_bytes+=16; // array flags
    size_bytes+=16; // dimensions flags
    size_bytes+=8+TinyMAT_DatElement_realstringlen8bit(name); // array name
    size_bytes+=8+TinyMAT_DatElement_realstringlen16bit(data); // actual data

    // write tag header
    TinyMAT_write32(mat, (uint32_t)TINYMAT_miMATRIX);
    TinyMAT_write32(mat, size_bytes);

    // write arrayflags
    TinyMAT_writeDatElement_u32a(mat, arrayflags, 2);

    // write field dimensions
    TinyMAT_write32(mat, (uint32_t)TINYMAT_miINT32);
    TinyMAT_write32(mat, (uint32_t)8);
    TinyMAT_write32(mat, 1);
    TinyMAT_write32(mat, slen);

    // write field name
    TinyMAT_writeDatElement_stringas8bit(mat, name);


    // write data type
    TinyMAT_writeDatElement_string(mat, data, slen);
}


void TinyMATWriter_writeMatrix2D_rowmajor(TinyMATWriterFile* mat, const char* name, const double* data_real, int32_t cols, int32_t rows) {
	uint32_t size_bytes=0;
    uint32_t arrayflags[2]={TINYMAT_mxDOUBLE_CLASS_arrayflags, 0};

	
	
	size_bytes+=16; // array flags
	size_bytes+=16; // dimensions flags
    size_bytes+=8+TinyMAT_DatElement_realstringlen8bit(name); // array name
	size_bytes+=8+8*cols*rows; // actual data
	
	// write tag header
    TinyMAT_write32(mat, (uint32_t)TINYMAT_miMATRIX);
    TinyMAT_write32(mat, size_bytes);
	
	// write arrayflags
    TinyMAT_writeDatElement_u32a(mat, arrayflags, 2);

	// write field dimensions
    TinyMAT_write32(mat, (uint32_t)TINYMAT_miINT32);
    TinyMAT_write32(mat, (uint32_t)8);
    TinyMAT_write32(mat, cols);
    TinyMAT_write32(mat, rows);
	
	// write field name
    TinyMAT_writeDatElement_stringas8bit(mat, name);

	// write data type
    TinyMAT_writeDatElement_dbla_rowmajor(mat, data_real, rows, cols);
	
}



void TinyMATWriter_close(TinyMATWriterFile* mat) {
	if (mat) {
        if (mat->file) TinyMAT_fclose(mat);
        //free(mat);
	}
}


#ifdef TINYMAT_USES_QVARIANT
    void TinyMATWriter_writeQVariantList(TinyMATWriterFile *mat, const char *name, const QVariantList &data)
    {
        uint32_t size_bytes=0;
        uint32_t arrayflags[2]={TINYMAT_mxCELL_CLASS_arrayflags, 0};





        // write tag header
        TinyMAT_write32(mat, (uint32_t)TINYMAT_miMATRIX);
        long sizepos=TinyMAT_ftell(mat);
        TinyMAT_write32(mat, size_bytes);

        // write arrayflags
        TinyMAT_writeDatElement_u32a(mat, arrayflags, 2);

        // write field dimensions
        TinyMAT_write32(mat, (uint32_t)TINYMAT_miINT32);
        TinyMAT_write32(mat, (uint32_t)8);
        TinyMAT_write32(mat, (int32_t)1);
        TinyMAT_write32(mat, (int32_t)data.size());

        // write field name
        TinyMAT_writeDatElement_stringas8bit(mat, name);

        // write data type
        for (int i=0; i<data.size(); i++) {
            if (data[i].type()==QVariant::String) {
                QByteArray a=data[i].toString().toLatin1();
                TinyMATWriter_writeString(mat, "", a.data(), a.size());
            } else if (data[i].canConvert(QVariant::Double)) {
                double a=data[i].toDouble();
                TinyMATWriter_writeMatrix2D_colmajor(mat, "", &a, 1, 1);
            } else if (data[i].canConvert(QVariant::PointF)) {
                double a[2]={data[i].toPointF().x(), data[i].toPointF().y()};
                TinyMATWriter_writeMatrix2D_colmajor(mat, "", a, 1, 2);
            } else if (data[i].canConvert(QVariant::Point)) {
                double a[2]={(double)data[i].toPoint().x(), (double)data[i].toPoint().y()};
                TinyMATWriter_writeMatrix2D_colmajor(mat, "", a, 1, 2);
            } else if (data[i].canConvert(QVariant::SizeF)) {
                double a[2]={data[i].toSizeF().width(), data[i].toSizeF().height()};
                TinyMATWriter_writeMatrix2D_colmajor(mat, "", a, 1, 2);
            } else if (data[i].canConvert(QVariant::Size)) {
                double a[2]={(double)data[i].toSize().width(), (double)data[i].toSize().height()};
                TinyMATWriter_writeMatrix2D_colmajor(mat, "", a, 1, 2);
            } else if (data[i].canConvert(QVariant::String)) {
                QByteArray a=data[i].toString().toLatin1();
                TinyMATWriter_writeString(mat, "", a.data(), a.size());
            } else {
                TinyMATWriter_writeMatrix2D_colmajor(mat, "", NULL, 0, 0);
            }
        }

        long endpos=TinyMAT_ftell(mat);
       TinyMAT_fseek(mat, sizepos, SEEK_SET);
        size_bytes=endpos-sizepos-4;
        TinyMAT_write32(mat, size_bytes);
       TinyMAT_fseek(mat, endpos, SEEK_SET);
    }


    void TinyMATWriter_writeQVariantMatrix(TinyMATWriterFile *mat, const char *name, const QList<QList<QVariant> > &data)
    {
        uint32_t size_bytes=0;
        uint32_t arrayflags[2]={TINYMAT_mxCELL_CLASS_arrayflags, 0};


        int32_t maxrows=0;
        for (int i=0; i<data.size(); i++) {
            for (int j=0; j<data[i].size(); j++) {
                maxrows=qMax(maxrows, data[i].size());
            }
        }


        // write tag header
        TinyMAT_write32(mat, (uint32_t)TINYMAT_miMATRIX);
        long sizepos;
        sizepos=TinyMAT_ftell(mat);
        TinyMAT_write32(mat, size_bytes);

        // write arrayflags
        TinyMAT_writeDatElement_u32a(mat, arrayflags, 2);

        // write field dimensions
        TinyMAT_write32(mat, (uint32_t)TINYMAT_miINT32);
        TinyMAT_write32(mat, (uint32_t)8);
        TinyMAT_write32(mat, (int32_t)maxrows);
        TinyMAT_write32(mat, (int32_t)data.size());

        // write field name
        TinyMAT_writeDatElement_stringas8bit(mat, name);

        // write data type
        for (int j=0; j<maxrows; j++) {
            for (int i=0; i<data.size(); i++) {
                QVariant v;
                if (j<data[i].size()) {
                    v=data[i].operator[](j);
                }
                //std::cout<<"+++ "<<i<<"/"<<j<<":   "<<TinyMAT_ftell(mat)<<" "<<(TinyMAT_ftell(mat)%8)<<"  write '"<<v.toString().toStdString()<<"'\n";
                if (v.type()==QVariant::String) {
                    QByteArray a=v.toString().toLatin1();
                    //std::cout<<i<<" "<<j<<" "<<QString(a).toStdString()<<"  length="<<a.size()<<"\n";
                    TinyMATWriter_writeString(mat, "", a.data(), a.size());
                } else if (v.canConvert(QVariant::Double)) {
                    double a=v.toDouble();
                    //std::cout<<i<<" "<<j<<" "<<a<<"\n";
                    TinyMATWriter_writeMatrix2D_colmajor(mat, "", &a, 1, 1);
                } else if (v.canConvert(QVariant::PointF)) {
                    double a[2]={v.toPointF().x(), v.toPointF().y()};
                    TinyMATWriter_writeMatrix2D_colmajor(mat, "", a, 1, 2);
                } else if (v.canConvert(QVariant::Point)) {
                    double a[2]={(double)v.toPoint().x(), (double)v.toPoint().y()};
                    TinyMATWriter_writeMatrix2D_colmajor(mat, "", a, 1, 2);
                } else if (v.canConvert(QVariant::SizeF)) {
                    double a[2]={v.toSizeF().width(), v.toSizeF().height()};
                    TinyMATWriter_writeMatrix2D_colmajor(mat, "", a, 1, 2);
                } else if (v.canConvert(QVariant::Size)) {
                    double a[2]={(double)v.toSize().width(), (double)v.toSize().height()};
                    TinyMATWriter_writeMatrix2D_colmajor(mat, "", a, 1, 2);
                } else if (v.canConvert(QVariant::String)) {
                    QByteArray a=v.toString().toLatin1();
                    TinyMATWriter_writeString(mat, "", a.data(), a.size());
                } else {
                    //std::cout<<i<<" "<<j<<" "<<"EMPTY"<<"\n";
                    TinyMATWriter_writeMatrix2D_colmajor(mat, "", NULL, 0, 0);
                }
                //std::cout<<"### "<<i<<"/"<<j<<":   "<<TinyMAT_ftell(mat)<<" "<<(TinyMAT_ftell(mat)%8)<<"\n";
            }
        }

        long endpos;
        endpos=TinyMAT_ftell(mat);
       TinyMAT_fseek(mat, sizepos, SEEK_SET);
        //fsetpos(mat->file, &sizepos);
        size_bytes=endpos-sizepos-4;
        TinyMAT_write32(mat, size_bytes);
       TinyMAT_fseek(mat, endpos, SEEK_SET);
        //fsetpos(mat->file, &endpos);
        //std::cout<<endpos<<" "<<TinyMAT_ftell(mat)<<"\n";
    }

#endif


