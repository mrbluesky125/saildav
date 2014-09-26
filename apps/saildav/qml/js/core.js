.pragma library

var _db;

function openDB()
{
    _db = openDatabaseSync("MeeDav","1.0","MeeDav Database",1000000);
    createAccountsTable();
}

function createAccountsTable()
{
    _db.transaction(
                function(tx){
                    tx.executeSql("CREATE TABLE IF NOT EXISTS accounts (id INTEGER PRIMARY KEY AUTOINCREMENT, url TEXT, username TEXT, password TEXT)");
                }
                )
}



function dropTables()
{
    _db.transaction(
                function(tx){
                    tx.executeSql("DROP TABLE IF EXISTS accounts");
                }
                )
}

function updateAccount(account)
{
    _db.transaction(
                function(tx){
                    tx.executeSql("UPDATE accounts SET username = ?, password = ?, url = ? \
                                  WHERE id = ?", [account.username, account.password, account.url, account.id]);
                }
                )
}

function deleteAccount(url)
{
    _db.transaction(
                function(tx){
                    tx.executeSql("DELETE FROM accounts WHERE url = ?", [url]);
                }
                )
}

function createAccount(account)
{
    _db.transaction(
                function(tx){
                    tx.executeSql("INSERT INTO accounts (url, username, password) VALUES(?,?,?)",[account.url, account.username, account.password]);
                }
                )
}

function getAccount() {
    var data = {}
    _db.readTransaction(
                function(tx){
                    var rs = tx.executeSql("SELECT * FROM accounts")
                    if(rs.rows.length === 1) {
                        data = rs.rows.item(0)
                    }
                }
                )
    return data;
}

function printObject(o)
{
    print(Object.keys(o))
    for(var key in o) {
        print("  " + key +"="+o[key])
    }

}

function defaultAccount()
{
    return { url: "http://", username: "", password: "" }
}
