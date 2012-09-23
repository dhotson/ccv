#include <Python.h>
#include "ccv.h"

static PyObject *
py_ccv_sift(PyObject *self, PyObject *args)
{
    const char *filename;
    int i, j;

    if (!PyArg_ParseTuple(args, "s", &filename))
        return NULL;

    ccv_enable_default_cache();
    ccv_dense_matrix_t* image = 0;
    ccv_read(filename, &image, CCV_IO_GRAY | CCV_IO_ANY_FILE);

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

    PyObject* keypoints = PyList_New(image_keypoints->rnum);
    PyObject* descriptors = PyList_New(image_keypoints->rnum);

    for (i = 0; i < image_keypoints->rnum; i++)
    {
        ccv_keypoint_t* op = (ccv_keypoint_t*)ccv_array_get(image_keypoints, i);
        PyList_SetItem(keypoints, i, Py_BuildValue("{s:f,s:f,s:d,s:d}",
            "x", op->x,
            "y", op->y,
            "scale", op->regular.scale,
            "angle", op->regular.angle
        ));

        PyObject* descriptor = PyList_New(128);
        float* desc = image_desc->data.f32 + i * 128;
        for (j = 0; j < 128; j++)
        {
            PyList_SetItem(descriptor, j, Py_BuildValue("f", desc[j]));
        }
        PyList_SetItem(descriptors, i, descriptor);
    }

    PyObject* ret = PyDict_New();
    PyDict_SetItemString(ret, "keypoints", keypoints);
    PyDict_SetItemString(ret, "descriptors", descriptors);

	ccv_array_free(image_keypoints);
	ccv_matrix_free(image_desc);
	ccv_matrix_free(image);
	ccv_disable_cache();
    return ret;
}

static PyMethodDef CCVMethods[] = {
    {"sift",  py_ccv_sift, METH_VARARGS,
     "Find features in an image using the SIFT algorithm"},
    {NULL, NULL, 0, NULL}        /* Sentinel */
};

PyMODINIT_FUNC
initccv(void)
{
    (void) Py_InitModule("ccv", CCVMethods);
}
