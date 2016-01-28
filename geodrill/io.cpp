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

        poBand = poDataset->GetRasterBand( 1 );
        poBand->GetBlockSize( &nBlockXSize, &nBlockYSize );

        printf( "Block=%dx%d Type=%s, ColorInterp=%s\n",
                nBlockXSize, nBlockYSize,
                GDALGetDataTypeName(poBand->GetRasterDataType()),
                GDALGetColorInterpretationName(
                        poBand->GetColorInterpretation()) );
        adfMinMax[0] = poBand->GetMinimum( &bGotMin );
        adfMinMax[1] = poBand->GetMaximum( &bGotMax );
        if( ! (bGotMin && bGotMax) )
            GDALComputeRasterMinMax((GDALRasterBandH)poBand, TRUE, adfMinMax);
        printf( "Min=%.3fd, Max=%.3f\n", adfMinMax[0], adfMinMax[1] );
        if( poBand->GetOverviewCount() > 0 )
            printf( "Band has %d overviews.\n", poBand->GetOverviewCount() );
        if( poBand->GetColorTable() != NULL )
            printf( "Band has a color table with %d entries.\n",
                    poBand->GetColorTable()->GetColorEntryCount() );
    }
    return 0;
}