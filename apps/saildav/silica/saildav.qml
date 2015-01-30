import QtQuick 2.0
import Sailfish.Silica 1.0

import "pages"
import "cover"

import org.bluesky.models 1.0

ApplicationWindow
{
    id: window

    SqlLiteModel {
         id: accountsModel
         databaseName: "saildav"
         tableName: "accounts"
         createStatement: "CREATE TABLE IF NOT EXISTS accounts(url TEXT, username TEXT, password TEXT)"
    }

    cover: Component { CoverPage {} }

    Component.onCompleted: {
         pageStack.push(Qt.resolvedUrl("pages/ChooseAccountPage.qml"))
    }
}


