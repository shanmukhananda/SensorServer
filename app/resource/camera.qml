import QtQuick 2.5
import QtMultimedia 5.5

Item {
    visible: true
    id: camera_view
    objectName: "camera_view"
    Camera {
        id: camera
        objectName: "camera"
    }

    VideoOutput {
        id: video_output
        objectName: "video_output"
        anchors.fill: parent
        source: camera
        autoOrientation: true
    }
}
