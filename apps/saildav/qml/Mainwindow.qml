import QtQuick 2.0
import QtQuick.Controls 1.1
import QtQuick.LocalStorage 2.0

import org.bluesky.basics 1.0
import org.bluesky.models 1.0
import qwebdav 1.0

Item {
    id: appWindow

    SqlLiteModel {
        id: accountModel
        databaseName: "saildav"
        databaseVersion: "1.0"
        tableName: "accounts"
        primaryKeyName: "id"
    }

    StackView {
        id: mainStackView
        anchors.fill: parent

        property alias accountModel: accountModel
    }

    Component.onCompleted: {
        var db = LocalStorage.openDatabaseSync("saildav", "1.0", "SailDAV Database", 1000000);
        db.transaction( function(tx) {
            tx.executeSql("CREATE TABLE IF NOT EXISTS accounts (id INTEGER PRIMARY KEY AUTOINCREMENT, url TEXT, username TEXT, password TEXT)");
        });
        accountModel.load();

        if(accountModel.count === 0) {
            accountModel.insertSql( { "url": "http://", "username": "", "password": "" } );
            mainStackView.push(Qt.resolvedUrl("AccountPage.qml"));
        }
        else {
            mainStackView.push(Qt.resolvedUrl("WebdavPage.qml"));
        }
    }

}
