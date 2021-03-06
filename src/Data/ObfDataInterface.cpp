#include "ObfDataInterface.h"
#include "ObfDataInterface_P.h"

#include "ObfReader.h"
#include "ObfInfo.h"
#include "ObfMapSectionReader.h"
#include "IQueryController.h"

OsmAnd::ObfDataInterface::ObfDataInterface( const QList< std::shared_ptr<ObfReader> >& readers )
    : _d(new ObfDataInterface_P(this, readers))
{
}

OsmAnd::ObfDataInterface::~ObfDataInterface()
{
}

void OsmAnd::ObfDataInterface::obtainObfFiles( QList< std::shared_ptr<const ObfFile> >* outFiles /*= nullptr*/, const IQueryController* const controller /*= nullptr*/ )
{
    for(const auto& obfReader : constOf(_d->readers))
    {
        if(controller && controller->isAborted())
            return;

        // Initialize OBF file
        obfReader->obtainInfo();

        if(outFiles)
            outFiles->push_back(obfReader->obfFile);
    }
}

void OsmAnd::ObfDataInterface::obtainBasemapPresenceFlag( bool& basemapPresent, const IQueryController* const controller /*= nullptr*/ )
{
    basemapPresent = false;
    for(const auto& obfReader : constOf(_d->readers))
    {
        if(controller && controller->isAborted())
            return;

        const auto& obfInfo = obfReader->obtainInfo();
        basemapPresent = basemapPresent || obfInfo->isBasemap;
    }
}

void OsmAnd::ObfDataInterface::obtainMapObjects(
    QList< std::shared_ptr<const OsmAnd::Model::MapObject> >* resultOut, MapFoundationType* foundationOut,
    const AreaI& area31, const ZoomLevel zoom,
    const IQueryController* const controller /*= nullptr*/, const FilterMapObjectsByIdSignature filterById /*= nullptr*/,
    ObfMapSectionReader_Metrics::Metric_loadMapObjects* const metric /*= nullptr*/)
{
    if(foundationOut)
        *foundationOut = MapFoundationType::Undefined;

    // Iterate through all OBF readers
    for(const auto& obfReader : constOf(_d->readers))
    {
        // Check if request is aborted
        if(controller && controller->isAborted())
            return;

        // Iterate over all map sections of each OBF reader
        const auto& obfInfo = obfReader->obtainInfo();
        for(const auto& mapSection : constOf(obfInfo->mapSections))
        {
            // Check if request is aborted
            if(controller && controller->isAborted())
                return;

            // Read objects from each map section
            OsmAnd::ObfMapSectionReader::loadMapObjects(obfReader, mapSection, zoom, &area31, resultOut, foundationOut, filterById, nullptr, controller, metric);
        }
    }
}
