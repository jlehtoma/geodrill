#include <iostream>
#include "gdal_priv.h"
#include "cpl_conv.h" // for CPLMalloc()
#include "io.h"

using namespace std;

int printRasterInfo(const char* pszFilename)
{
    GDALDataset *poDataset;
    GDALAllRegister();
    poDataset = (GDALDataset *) GDALOpen( pszFilename, GA_ReadOnly );

    if( poDataset == NULL )
    {
        std::cout << "Dataset " << pszFilename << " could not be opened.";
        return -1;
    }
    return 0;
}
