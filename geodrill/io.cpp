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
        std::cout << "Dataset " << pszFilename << " could not be opened." << std::endl;
        return -1;
    }
    else
    {
        double adfGeoTransform[6];
        std::cout << " Driver: " << poDataset->GetDriver()->GetDescription() << "/" <<
                poDataset->GetDriver()->GetMetadataItem( GDAL_DMD_LONGNAME ) << std::endl;
        std::cout << " Size: " << poDataset->GetRasterXSize() << "x" << poDataset->GetRasterYSize() << "x" <<
                poDataset->GetRasterCount() << std::endl;
        if( poDataset->GetProjectionRef()  != NULL )
            std::cout << " Projection: " << poDataset->GetProjectionRef() << std::endl;
        if( poDataset->GetGeoTransform( adfGeoTransform ) == CE_None )
        {
            std::cout << " Origin: (" << adfGeoTransform[0] << ", " << adfGeoTransform[3] << ")" << std::endl;
            std::cout << " Pixel Size: (" << adfGeoTransform[1] << ", " << adfGeoTransform[5] << std::endl;
        }
    }
    return 0;
}
