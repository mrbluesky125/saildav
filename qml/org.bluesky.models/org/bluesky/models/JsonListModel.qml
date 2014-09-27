//:start<bJsonListModel>
//:start<bJsonListModel.properties>
import QtQuick 2.2
import "jsonpath.js" as JSONPath

ListModel {
    id: jsonListModel

    property string source
    property string query
    property string status: "Null"
    property string json
//:end<bJsonListModel.properties>

    onSourceChanged: reload()
    onJsonChanged: updateModel()
    onQueryChanged: updateModel()

    //:start<bJsonListModel.reload>
    function reload() {
        if(jsonListModel.source === "") return;

        jsonListModel.status = "Loading";

        var xhr = new XMLHttpRequest;
        xhr.open("GET", jsonListModel.source);
        xhr.onreadystatechange = function() {
            if (xhr.readyState === XMLHttpRequest.DONE) {
                if(xhr.status === 200) {
                    jsonListModel.json = xhr.responseText;
                }
                else {
                    console.log("XMLHttpRequest unknown status (" + xhr.status + ") with (" + source + ") as source URL.");
                    jsonListModel.status = "Null";
                }
            }
        }
        xhr.onerror = function() {
            jsonListModel.status = "Error";
            console.log("XMLHttpRequest has caused an error with (" + source + ") as source URL.")
        }
        xhr.send();
    }
    //:end<bJsonListModel.reload>

    //:start<bJsonListModel.update>
    function updateModel() {
        jsonListModel.clear();

        if ( jsonListModel.json === "" ) {
            jsonListModel.status = "Null";
            return;
        }

        var jsonObject = JSON.parse(jsonListModel.json);
        if(jsonListModel.query !== "")
            jsonObject = JSONPath.jsonPath(jsonObject, jsonListModel.query)

        jsonListModel.append( jsonObject );
        jsonListModel.status = "Ready";
    }
    //:end<bJsonListModel.update>

    Component.onCompleted: reload()
}
//:end<bJsonListModel>
