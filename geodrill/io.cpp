#include <iostream>
#include "gdal_priv.h"
#include "cpl_conv.h" // for CPLMalloc()
#include "io.h"

using namespace std;

/*! \file io.cpp
    With a little bit of a elaboration, should you feel it necessary.
*/
int printRasterInfo(const char* pszFilename)
{
    /** An enum type.
     *  The documentation block cannot be put after the enum!
     */

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
    // Set config options for GDAL (needs >= 2.0). Setting GTIFF_VIRTUAL_MEM_IO to "YES" can cause things bleed to
    // swap if enough RAM is not available. Use "IF_ENOUGH_RAM" for safer performance if unsure. NOTE that faster
    // mem io only works with *uncompressed* GeoTIFFs.
    // New in GDAL 2.0, from https://2015.foss4g-na.org/sites/default/files/slides/GDAL%202.0%20overview.pdf
    // GeoTIFF driver (with i7-4700 HQ (8 vCPUs)):with i7-4700 HQ (8 vCPUs)
    //   - Default: time ./testblockcache -ondisk: 7.5s
    //   - GTIFF_DIRECT_IO=YES: short circuit the block cache&libtiff for most RasterIO() operations (restricted to
    //     uncompressed stripped GeoTIFF). time ./testblockcache -ondisk: 2s
    //   - GTIFF_VIRTUAL_MEM_IO=YES: same as above, with tiled GeoTIFF as well. Uses memory-mapped file access. Linux
    //     only for now, 64bit recommended (could be extended to other platforms possible).
    //     time ./testblockcache -ondisk: 0.3s
    //
    CPLSetConfigOption("GTIFF_VIRTUAL_MEM_IO", "YES" );
    // Open the dataset
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

        //  Get raster band and its size
        poBand = poDataset->GetRasterBand(1);
        poBand->GetBlockSize( &nBlockXSize, &nBlockYSize);

        std::cout << "Dataset: " << pszFilename << std::endl;

        std::cout << "Block=" << nBlockXSize << "x" << nBlockYSize << " Type=" <<
                GDALGetDataTypeName(poBand->GetRasterDataType()) << " ColorInterp=" <<
                GDALGetColorInterpretationName(poBand->GetColorInterpretation()) << std::endl;

        // Calculate some stats
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

        // Get the actual data
        float *pafScanline;
        int   nXSize = poBand->GetXSize();
        pafScanline = (float *) CPLMalloc(sizeof(float) * nXSize);

        // RasterIO has a new argument psExtraArg in GDAL > 2.0. NOTE: that  GDALRasterBand::ReadBlock() probably has
        // better performance for reading the whole data at one go.
        #ifdef USE_GDAL_2
            GDALRasterIOExtraArg* arg = NULL;
            poBand->RasterIO(GF_Read, 0, 0, nXSize, 1, pafScanline, nXSize, 1, GDT_Float3GDALRasterBand::ReadBlock 2,
               0, 0, arg);
        #else
            poBand->RasterIO(GF_Read, 0, 0, nXSize, 1, pafScanline, nXSize, 1, GDT_Float32, 0, 0);
        #endif

        // ... do something with the data ...

        // Free resources
        CPLFree(pafScanline);
        GDALClose((GDALDatasetH) poDataset);
        std::cout << std::endl;
    }
    return 0;
}
