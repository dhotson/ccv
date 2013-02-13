#include "ccv.h"

typedef struct {
	float* descriptor;
	float x;
	float y;
	int octave;
	int level;
	double scale;
	double angle;
} sift_feature;


ccv_dense_matrix_t* simple_ccv_read(char* filename)
{
	ccv_dense_matrix_t* image = 0;
	ccv_read(filename, &image, CCV_IO_GRAY | CCV_IO_ANY_FILE);
	return image;
}

sift_feature* simple_ccv_sift(ccv_dense_matrix_t* image, int* n)
{
	ccv_sift_param_t params = {
		.noctaves = 3,
		.nlevels = 6,
		.up2x = 1,
		.edge_threshold = 10,
		.norm_threshold = 0,
		.peak_threshold = 0,
	};
	ccv_array_t* image_keypoints = 0;
	ccv_dense_matrix_t* image_desc = 0;

	ccv_sift(image, &image_keypoints, &image_desc, 0, params);

	sift_feature* features;
	if (0 == (features = malloc(sizeof(sift_feature) * image_keypoints->rnum)))
		return NULL;
	int i;
	*n = image_keypoints->rnum;
	for (i = 0; i < image_keypoints->rnum; i++)
	{
		ccv_keypoint_t* kp = (ccv_keypoint_t*)ccv_array_get(image_keypoints, i);
		features[i].descriptor = image_desc->data.f32 + i * 128;
		features[i].x = kp->x;
		features[i].y = kp->x;
		features[i].octave = kp->octave;
		features[i].level = kp->level;
		features[i].scale = kp->regular.scale;
		features[i].angle = kp->regular.angle;
	}

	return features;
}


