import QtQuick 2.5
import QtQuick.Controls 2.1
import QtMultimedia 5.5
import QtQuick.Window 2.2
import QQuickCamera 1.0

Window {
    visible: true
    id: app_window
    objectName: "app_window"
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
