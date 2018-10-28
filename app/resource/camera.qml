import QtQuick 2.5
import QtMultimedia 5.5
import SensorServer 1.0

Item {
    visible: true
    id: camera_view
    objectName: "camera_view"
    Camera {
        id: camera
        objectName: "camera"
        viewfinder.resolution: "640x480"
    }

    VideoOutput {
        id: video_output
        objectName: "video_output"
        anchors.fill: parent
        source: camera
        filters: [ video_filter ]
        autoOrientation: true
    }

    VideoFilter {
        id: video_filter
        objectName: "video_filter"
    }
}
