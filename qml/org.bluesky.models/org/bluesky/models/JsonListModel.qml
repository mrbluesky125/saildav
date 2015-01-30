/*
 * Copyright (C) 2012 Timo Zimmermann <zimmermann.emb@googlemail.com>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public License
 * along with this library; see the file COPYING.LIB.  If not, write to
 * the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA 02110-1301, USA.
 */

import QtQuick 2.0
import "jsonpath.js" as JSONPath

ListModel {
    id: jsonListModel

    property string source
    property string query
    property string status: "Null"
    property string json

    onSourceChanged: reload()
    onJsonChanged: updateModel()
    onQueryChanged: updateModel()

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

    Component.onCompleted: reload()
}

