#include <iostream>
#include "gdal_priv.h"
#include "cpl_conv.h" // for CPLMalloc()
#include "io.h"

using namespace std;

/**
* @brief A short function description
*
* A more detailed function description
*/

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
        std::cout << "Dataset: " << pszFilename << std::endl;
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

int readRaster(const char* pszFilename)
{
    CPLSetConfigOption("GTIFF_VIRTUAL_MEM_IO", "YES" );
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
        GDALRasterBand  *poBand;
        int             nBlockXSize, nBlockYSize;
        int             bGotMin, bGotMax;
        double          adfMinMax[2];

        poBand = poDataset->GetRasterBand(1);
        poBand->GetBlockSize( &nBlockXSize, &nBlockYSize);

        std::cout << "Dataset: " << pszFilename << std::endl;

        std::cout << "Block=" << nBlockXSize << "x" << nBlockYSize << " Type=" <<
                GDALGetDataTypeName(poBand->GetRasterDataType()) << " ColorInterp=" <<
                GDALGetColorInterpretationName(poBand->GetColorInterpretation()) << std::endl;

        adfMinMax[0] = poBand->GetMinimum(&bGotMin);
        adfMinMax[1] = poBand->GetMaximum(&bGotMax);

        if(!(bGotMin && bGotMax)) {
            GDALComputeRasterMinMax((GDALRasterBandH) poBand, TRUE, adfMinMax);
        }
        std::cout << "Min=" << adfMinMax[0] << " Max=" << adfMinMax[1] << std::endl;

        if(poBand->GetOverviewCount() > 0) {
            std::cout << "Band has " << poBand->GetOverviewCount() << " overviews." << std::endl;
        }
        if( poBand->GetColorTable() != NULL ) {
            std::cout << "Band has a color table with " << poBand->GetColorTable()->GetColorEntryCount() <<
                    " entries." << std::endl;
        }

        float *pafScanline;
        int   nXSize = poBand->GetXSize();
        pafScanline = (float *) CPLMalloc(sizeof(float) * nXSize);

        // RasterIO has a new argument psExtraArg in GDAL > 2.0
        #ifdef USE_GDAL_2
            GDALRasterIOExtraArg* arg = NULL;function call arguments different library version
            poBand->RasterIO(GF_Read, 0, 0, nXSize, 1, pafScanline, nXSize, 1, GDT_Float32, 0, 0, arg);
        #else
            poBand->RasterIO(GF_Read, 0, 0, nXSize, 1, pafScanline, nXSize, 1, GDT_Float32, 0, 0);
        #endif

        CPLFree(pafScanline);
        GDALClose((GDALDatasetH) poDataset);
        std::cout << std::endl;
    }
    return 0;
}