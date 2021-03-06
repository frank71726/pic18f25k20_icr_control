#ifndef _ASX340_PARA_H
#define _ASX340_PARA_H

//========================================================
/*
#define     GAIN_HI	0x30
#define     GAIN_LO	0x5E
#define     EXPO_HI	0x30
#define     EXPO_LO	0x12
*/
//========================================================
#define	    ASX340_NUM		16//11+5(parael out)
#define	    ASX340_DATA		29//21+8(parael out)
#define     ASX340_HMIRROR_NUM	5
#define     ASX340_HMIRROR_DATA	10
//===============================================================

#if _HI_TECH

const INT8U ASX340_hi[ASX340_NUM]={
	0x00, 0x00, 0x00, 0x00, 0x09, 0xC8, 0x98, 0xFC, 0x00, 0x00, 
	0x00, 
	0x09, 0xC9, 0x94, 0x98, 0xC9//parael out 
};
const INT8U ASX340_lo[ASX340_NUM]={
	0x1A, 0x1A, 0x40, 0x40, 0x8E, 0x58, 0x26, 0x00, 0x40, 0x40, 
	0x1E, 
	0x8E, 0x72, 0x26, 0x26, 0x72//parael out
};
const INT8U ASX340_data[ASX340_DATA]={
	0x00, 0x01, 0x00, 0x00, 0x81, 0x01, 0x81, 0x01, 0x48, 0x58, 
	0x00, 0x02, 0x23, 0x28, 0x00, 0x81, 0x00, 0x81, 0x01, 0x03, 0x02,
	0x49, 0x72, 0x00, 0x03, 0x00, 0x00, 0x00, 0x20//parael out
};
const INT8U ASX340_num[ASX340_NUM]={
	0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x01, 0x02, 0x02, 0x02, 
	0x02,
	0x02, 0x02, 0x01, 0x01, 0x02
};
//===============================================================
const INT8U ASX340_mirror_hi[ASX340_HMIRROR_NUM]={
	0x09, 0xC8, 0xFC, 0x00, 0x00
};
const INT8U ASX340_mirror_lo[ASX340_HMIRROR_NUM]={
	0x8E, 0x38, 0x00, 0x40, 0x40
};
const INT8U ASX340_mirror_data[ASX340_HMIRROR_DATA]={
	0x48, 0x38, 0x00, 0x01, 0x28, 0x00, 0x81, 0x00, 0x81, 0x01
};
const INT8U ASX340_mirror_num[ASX340_HMIRROR_NUM]={
	0x02, 0x02, 0x02, 0x02, 0x02
};

const INT8U ASX340_no_mirror_data[ASX340_HMIRROR_DATA]={
	0x48, 0x38, 0x00, 0x00, 0x28, 0x00, 0x81, 0x00, 0x81, 0x01
};
//===============================================================
#elif	_PIC_C18

#pragma romdata


rom INT8U const ASX340_hi[ASX340_NUM]={
	0x00, 0x00, 0x00, 0x00, 0x09, 0xC8, 0x98, 0xFC, 0x00, 0x00, 
	0x00, 
	0x09, 0xC9, 0x94, 0x98, 0xC9//parael out 
};
rom INT8U const ASX340_lo[ASX340_NUM]={
	0x1A, 0x1A, 0x40, 0x40, 0x8E, 0x58, 0x26, 0x00, 0x40, 0x40, 
	0x1E, 
	0x8E, 0x72, 0x26, 0x26, 0x72//parael out
};
rom INT8U const ASX340_data[ASX340_DATA]={
	0x00, 0x01, 0x00, 0x00, 0x81, 0x01, 0x81, 0x01, 0x48, 0x58, 
	0x00, 0x02, 0x23, 0x28, 0x00, 0x81, 0x00, 0x81, 0x01, 0x03, 0x02,
	0x49, 0x72, 0x00, 0x03, 0x00, 0x00, 0x00, 0x20//parael out
};
rom INT8U const ASX340_num[ASX340_NUM]={
	0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x01, 0x02, 0x02, 0x02, 
	0x02,
	0x02, 0x02, 0x01, 0x01, 0x02
};
//===============================================================
rom INT8U const ASX340_mirror_hi[ASX340_HMIRROR_NUM]={
	0x09, 0xC8, 0xFC, 0x00, 0x00
};
rom INT8U const ASX340_mirror_lo[ASX340_HMIRROR_NUM]={
	0x8E, 0x38, 0x00, 0x40, 0x40
};
rom INT8U const ASX340_mirror_data[ASX340_HMIRROR_DATA]={
	0x48, 0x38, 0x00, 0x01, 0x28, 0x00, 0x81, 0x00, 0x81, 0x01
};
rom INT8U const ASX340_mirror_num[ASX340_HMIRROR_NUM]={
	0x02, 0x02, 0x02, 0x02, 0x02
};

rom INT8U const ASX340_no_mirror_data[ASX340_HMIRROR_DATA]={
	0x48, 0x38, 0x00, 0x00, 0x28, 0x00, 0x81, 0x00, 0x81, 0x01
};
//===============================================================
#pragma romdata

#else
//none
#endif

#endif
