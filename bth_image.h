const unsigned int BTH_IMAGE_WIDTH = 1;
const unsigned int BTH_IMAGE_HEIGHT = 1;

//Image data stored in 8-bit RGBA format

//unsigned char BTH_IMAGE_DATA[] = {
//	255, 0, 0, 1, 0, 255, 0, 1,
//	0, 0, 255, 1, 255, 255, 255, 1,
//};

unsigned char BTH_IMAGE_DATA[] = {
	255, 255, 255, 1,
};

//unsigned char BTH_IMAGE_DATA[] = {
//		0, 0, 0, 1, 255, 255, 255, 1,
//		0, 0, 0, 1, 255, 255, 255, 1,
//		0, 0, 0, 1, 255, 255, 255, 1,
//		0, 0, 0, 1, 255, 255, 255, 1,
//		0, 0, 0, 1, 255, 255, 255, 1,
//		0, 0, 0, 1, 255, 255, 255, 1,
//		0, 0, 0, 1, 255, 255, 255, 1,
//		0, 0, 0, 1, 255, 255, 255, 1,
//	
//		255, 255, 255, 1, 0, 0, 0, 1,
//		255, 255, 255, 1, 0, 0, 0, 1,
//		255, 255, 255, 1, 0, 0, 0, 1,
//		255, 255, 255, 1, 0, 0, 0, 1,
//		255, 255, 255, 1, 0, 0, 0, 1,
//		255, 255, 255, 1, 0, 0, 0, 1,
//		255, 255, 255, 1, 0, 0, 0, 1,
//		255, 255, 255, 1, 0, 0, 0, 1,
//
//		0, 0, 0, 1, 255, 255, 255, 1,
//		0, 0, 0, 1, 255, 255, 255, 1,
//		0, 0, 0, 1, 255, 255, 255, 1,
//		0, 0, 0, 1, 255, 255, 255, 1,
//		0, 0, 0, 1, 255, 255, 255, 1,
//		0, 0, 0, 1, 255, 255, 255, 1,
//		0, 0, 0, 1, 255, 255, 255, 1,
//		0, 0, 0, 1, 255, 255, 255, 1,
//
//		255, 255, 255, 1, 0, 0, 0, 1,
//		255, 255, 255, 1, 0, 0, 0, 1,
//		255, 255, 255, 1, 0, 0, 0, 1,
//		255, 255, 255, 1, 0, 0, 0, 1,
//		255, 255, 255, 1, 0, 0, 0, 1,
//		255, 255, 255, 1, 0, 0, 0, 1,
//		255, 255, 255, 1, 0, 0, 0, 1,
//		255, 255, 255, 1, 0, 0, 0, 1,
//	
//		0, 0, 0, 1, 255, 255, 255, 1,
//		0, 0, 0, 1, 255, 255, 255, 1,
//		0, 0, 0, 1, 255, 255, 255, 1,
//		0, 0, 0, 1, 255, 255, 255, 1,
//		0, 0, 0, 1, 255, 255, 255, 1,
//		0, 0, 0, 1, 255, 255, 255, 1,
//		0, 0, 0, 1, 255, 255, 255, 1,
//		0, 0, 0, 1, 255, 255, 255, 1,
//
//		255, 255, 255, 1, 0, 0, 0, 1,
//		255, 255, 255, 1, 0, 0, 0, 1,
//		255, 255, 255, 1, 0, 0, 0, 1,
//		255, 255, 255, 1, 0, 0, 0, 1,
//		255, 255, 255, 1, 0, 0, 0, 1,
//		255, 255, 255, 1, 0, 0, 0, 1,
//		255, 255, 255, 1, 0, 0, 0, 1,
//		255, 255, 255, 1, 0, 0, 0, 1,
//	
//		0, 0, 0, 1, 255, 255, 255, 1,
//		0, 0, 0, 1, 255, 255, 255, 1,
//		0, 0, 0, 1, 255, 255, 255, 1,
//		0, 0, 0, 1, 255, 255, 255, 1,
//		0, 0, 0, 1, 255, 255, 255, 1,
//		0, 0, 0, 1, 255, 255, 255, 1,
//		0, 0, 0, 1, 255, 255, 255, 1,
//		0, 0, 0, 1, 255, 255, 255, 1,
//	
//		255, 255, 255, 1, 0, 0, 0, 1,
//		255, 255, 255, 1, 0, 0, 0, 1,
//		255, 255, 255, 1, 0, 0, 0, 1,
//		255, 255, 255, 1, 0, 0, 0, 1,
//		255, 255, 255, 1, 0, 0, 0, 1,
//		255, 255, 255, 1, 0, 0, 0, 1,
//		255, 255, 255, 1, 0, 0, 0, 1,
//		255, 255, 255, 1, 0, 0, 0, 1,
//	
//		0, 0, 0, 1, 255, 255, 255, 1,
//		0, 0, 0, 1, 255, 255, 255, 1,
//		0, 0, 0, 1, 255, 255, 255, 1,
//		0, 0, 0, 1, 255, 255, 255, 1,
//		0, 0, 0, 1, 255, 255, 255, 1,
//		0, 0, 0, 1, 255, 255, 255, 1,
//		0, 0, 0, 1, 255, 255, 255, 1,
//		0, 0, 0, 1, 255, 255, 255, 1,
//
//		255, 255, 255, 1, 0, 0, 0, 1,
//		255, 255, 255, 1, 0, 0, 0, 1,
//		255, 255, 255, 1, 0, 0, 0, 1,
//		255, 255, 255, 1, 0, 0, 0, 1,
//		255, 255, 255, 1, 0, 0, 0, 1,
//		255, 255, 255, 1, 0, 0, 0, 1,
//		255, 255, 255, 1, 0, 0, 0, 1,
//		255, 255, 255, 1, 0, 0, 0, 1,
//	
//		0, 0, 0, 1, 255, 255, 255, 1,
//		0, 0, 0, 1, 255, 255, 255, 1,
//		0, 0, 0, 1, 255, 255, 255, 1,
//		0, 0, 0, 1, 255, 255, 255, 1,
//		0, 0, 0, 1, 255, 255, 255, 1,
//		0, 0, 0, 1, 255, 255, 255, 1,
//		0, 0, 0, 1, 255, 255, 255, 1,
//		0, 0, 0, 1, 255, 255, 255, 1,
//	
//		255, 255, 255, 1, 0, 0, 0, 1,
//		255, 255, 255, 1, 0, 0, 0, 1,
//		255, 255, 255, 1, 0, 0, 0, 1,
//		255, 255, 255, 1, 0, 0, 0, 1,
//		255, 255, 255, 1, 0, 0, 0, 1,
//		255, 255, 255, 1, 0, 0, 0, 1,
//		255, 255, 255, 1, 0, 0, 0, 1,
//		255, 255, 255, 1, 0, 0, 0, 1,
//
//		0, 0, 0, 1, 255, 255, 255, 1,
//		0, 0, 0, 1, 255, 255, 255, 1,
//		0, 0, 0, 1, 255, 255, 255, 1,
//		0, 0, 0, 1, 255, 255, 255, 1,
//		0, 0, 0, 1, 255, 255, 255, 1,
//		0, 0, 0, 1, 255, 255, 255, 1,
//		0, 0, 0, 1, 255, 255, 255, 1,
//		0, 0, 0, 1, 255, 255, 255, 1,
//
//		255, 255, 255, 1, 0, 0, 0, 1,
//		255, 255, 255, 1, 0, 0, 0, 1,
//		255, 255, 255, 1, 0, 0, 0, 1,
//		255, 255, 255, 1, 0, 0, 0, 1,
//		255, 255, 255, 1, 0, 0, 0, 1,
//		255, 255, 255, 1, 0, 0, 0, 1,
//		255, 255, 255, 1, 0, 0, 0, 1,
//		255, 255, 255, 1, 0, 0, 0, 1,
//
//		0, 0, 0, 1, 255, 255, 255, 1,
//		0, 0, 0, 1, 255, 255, 255, 1,
//		0, 0, 0, 1, 255, 255, 255, 1,
//		0, 0, 0, 1, 255, 255, 255, 1,
//		0, 0, 0, 1, 255, 255, 255, 1,
//		0, 0, 0, 1, 255, 255, 255, 1,
//		0, 0, 0, 1, 255, 255, 255, 1,
//		0, 0, 0, 1, 255, 255, 255, 1,
//
//		255, 255, 255, 1, 0, 0, 0, 1,
//		255, 255, 255, 1, 0, 0, 0, 1,
//		255, 255, 255, 1, 0, 0, 0, 1,
//		255, 255, 255, 1, 0, 0, 0, 1,
//		255, 255, 255, 1, 0, 0, 0, 1,
//		255, 255, 255, 1, 0, 0, 0, 1,
//		255, 255, 255, 1, 0, 0, 0, 1,
//		255, 255, 255, 1, 0, 0, 0, 1,
//};
