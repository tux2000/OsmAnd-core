#ifndef _OSMAND_CORE_OBF_MAP_SECTION_READER_P_H_
#define _OSMAND_CORE_OBF_MAP_SECTION_READER_P_H_

#include <OsmAndCore/stdlib_common.h>
#include <functional>

#include <OsmAndCore/QtExtensions.h>
#include <QHash>
#include <QMap>
#include <QSet>

#include <OsmAndCore.h>
#include <CommonTypes.h>
#include <MapTypes.h>

namespace OsmAnd {

    class ObfReader_P;
    class ObfMapSectionInfo;
    class ObfMapSectionLevel;
    struct ObfMapSectionDecodingEncodingRules;
    class ObfMapSectionLevelTreeNode;
    namespace Model {
        class MapObject;
    } // namespace Model
    class IQueryController;
    namespace ObfMapSectionReader_Metrics {
        struct Metric_loadMapObjects;
    } // namespace ObfMapSectionReader_Metrics

    class ObfMapSectionReader;
    class OSMAND_CORE_API ObfMapSectionReader_P
    {
    private:
        ObfMapSectionReader_P();
        ~ObfMapSectionReader_P();

    protected:
        static void read(const std::unique_ptr<ObfReader_P>& reader, const std::shared_ptr<ObfMapSectionInfo>& section);

        static void readMapLevelHeader(const std::unique_ptr<ObfReader_P>& reader, const std::shared_ptr<const ObfMapSectionInfo>& section,
            const std::shared_ptr<ObfMapSectionLevel>& level);

        static void readEncodingDecodingRules(const std::unique_ptr<ObfReader_P>& reader,
            const std::shared_ptr<ObfMapSectionDecodingEncodingRules>& encodingDecodingRules);
        static void readEncodingDecodingRule(const std::unique_ptr<ObfReader_P>& reader,
            uint32_t defaultId, const std::shared_ptr<ObfMapSectionDecodingEncodingRules>& encodingDecodingRules);

        static void readMapLevelTreeNodes(const std::unique_ptr<ObfReader_P>& reader, const std::shared_ptr<const ObfMapSectionInfo>& section,
            const std::shared_ptr<const ObfMapSectionLevel>& level, QList< std::shared_ptr<ObfMapSectionLevelTreeNode> >& nodes);
        static void readTreeNode(const std::unique_ptr<ObfReader_P>& reader, const std::shared_ptr<const ObfMapSectionInfo>& section,
            const AreaI& parentArea,
            const std::shared_ptr<ObfMapSectionLevelTreeNode>& treeNode);
        static void readTreeNodeChildren(const std::unique_ptr<ObfReader_P>& reader, const std::shared_ptr<const ObfMapSectionInfo>& section,
            const std::shared_ptr<ObfMapSectionLevelTreeNode>& treeNode,
            MapFoundationType& foundation,
            QList< std::shared_ptr<ObfMapSectionLevelTreeNode> >* nodesWithData,
            const AreaI* bbox31,
            const IQueryController* const controller,
            ObfMapSectionReader_Metrics::Metric_loadMapObjects* const metric);

        static void readMapObjectsBlock(const std::unique_ptr<ObfReader_P>& reader, const std::shared_ptr<const ObfMapSectionInfo>& section,
            const std::shared_ptr<ObfMapSectionLevelTreeNode>& treeNode,
            QList< std::shared_ptr<const OsmAnd::Model::MapObject> >* resultOut,
            const AreaI* bbox31,
            const FilterMapObjectsByIdSignature filterById,
            std::function<bool (const std::shared_ptr<const OsmAnd::Model::MapObject>&)> visitor,
            const IQueryController* const controller,
            ObfMapSectionReader_Metrics::Metric_loadMapObjects* const metric);

        static void readMapObjectId(const std::unique_ptr<ObfReader_P>& reader, const std::shared_ptr<const ObfMapSectionInfo>& section,
            uint64_t baseId,
            uint64_t& objectId);

        static void readMapObject(const std::unique_ptr<ObfReader_P>& reader, const std::shared_ptr<const ObfMapSectionInfo>& section,
            uint64_t baseId, const std::shared_ptr<ObfMapSectionLevelTreeNode>& treeNode,
            std::shared_ptr<OsmAnd::Model::MapObject>& mapObjectOut,
            const AreaI* bbox31);

        enum {
            ShiftCoordinates = 5,
            MaskToRead = ~((1u << ShiftCoordinates) - 1),
        };

        static void loadMapObjects(const std::unique_ptr<ObfReader_P>& reader, const std::shared_ptr<const ObfMapSectionInfo>& section,
            ZoomLevel zoom, const AreaI* bbox31,
            QList< std::shared_ptr<const OsmAnd::Model::MapObject> >* resultOut, MapFoundationType* foundationOut,
            const FilterMapObjectsByIdSignature filterById,
            std::function<bool (const std::shared_ptr<const OsmAnd::Model::MapObject>&)> visitor,
            const IQueryController* const controller,
            ObfMapSectionReader_Metrics::Metric_loadMapObjects* const metric);

    friend class OsmAnd::ObfMapSectionReader;
    friend class OsmAnd::ObfReader_P;
    };

} // namespace OsmAnd

#endif // !defined(_OSMAND_CORE_OBF_MAP_SECTION_READER_P_H_)
