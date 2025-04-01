#############################################
# MySQL迁移MySQL源库中表的保持大小写的列名到Halo[MySQL]中
# 
# 脚本名称： 
# 	migration_mysql_column_names_with_case.py
# 
# 脚本运行环境要求： 
# 	python3（不低于3.6）
#   具备相关运行时环境
# 
# 脚本运行方式：
#   首先打开本文件，修改下面MySQL源库和Halo[MySQL]目标库的连接信息，和待迁移数据库名称。
# 	python  migration_mysql_column_names_with_case.py
#   如果启动后报类似如下错误：
#       File "convert_mysqldump_file.py", line 2
#       SyntaxError: Non-ASCII character '\xe5' in file convert_mysqldump_file.py on line 2, but no encoding declared; see http://python.org/dev/peps/pep-0263/ for details
#       则说明Python版本太低。
# 
# 脚本运行结果：
#   mys_informa_schema.schema_table_columns表中会重新写入数据。
# 
# 本脚本可以重复执行，每次启动执行后，会先清除mys_informa_schema.schema_table_columns表中的数据。
#############################################


import os
from re import S
import time
import datetime 
import sys
import platform
import shutil
import difflib

import pymysql
from pymysql.constants import CLIENT


# 以下配置信息，请根据实际环境修改##########################
# 源库，也就是官方MySQL数据库相关的连接参数
#mysqlHost = "?"
#mysqlPort = ?
#mysqlUser = "?"
#mysqlPassword = "?"

# pg_mysql 参数
# 目标库，也就是Halo[MySQL]数据库相关的连接参数
#haloMySQLHost = "?"
#haloMySQLPort = ?
#haloMySQLUser = "?"
#haloMySQLPassword = "?"

#业务数据库名称
businessDBs = ""
###########################################################


def checkPythonVersion():
    versionInfo = platform.python_version_tuple();
    if ((versionInfo[0] < '3') or (versionInfo[1] < '6')):
        print("Python版本过低，请使用3.6或更高版本的Python！")
        sys.exit(0)


def startTransaction(conn):
    cursor = conn.cursor()
    sql = "begin"
    try:
        cursor.execute(sql)
    except Exception as ex:
        print(f"    启动事务失败（{str(ex)}），程序终止。")
        sys.exit(0)
    cursor.close()


def commitTransaction(conn):
    cursor = conn.cursor()
    sql = "commit"
    try:
        cursor.execute(sql)
    except Exception as ex:
        print(f"    提交事务失败（{str(ex)}），程序终止。")
        sys.exit(0)
    cursor.close()


def rollbackTransaction(conn):
    cursor = conn.cursor()
    sql = "rollback"
    try:
        cursor.execute(sql)
    except Exception as ex:
        print(f"    回滚事务失败（{str(ex)}），程序终止。")
        sys.exit(0)
    cursor.close()


def clearSchemaTableColumns(conn, scheName):
    if (scheName == "information_schema"):
        scheName = "mys_informa_schema"

    cursor = conn.cursor()
    sql = "delete from mys_informa_schema.schema_table_columns where schema_name = '" + scheName + "'";
    try:
        cursor.execute(sql)
        conn.commit();
    except Exception as ex:
        print(f"    访问数据库表'mys_informa_schema.schema_table_columns'遇到错误（{str(ex)}）'，程序终止。")
        rollbackTransaction(haloConn)
        sys.exit(0)
    cursor.close()


def runUseDatabase(conn, scheName):
    if (scheName == "mys_informa_schema"):
        scheName = "information_schema"

    cursor = conn.cursor()
    sql = "use " + scheName
    try:
        cursor.execute(sql)
    except Exception as ex:
        print(f"    目标数据库'{scheName}'不存在或无法访问（{str(ex)}）'，程序终止。")
        rollbackTransaction(haloConn)
        sys.exit(0)
    cursor.close()


def runShowTables(conn, scheName):
    tabNames = []
    cursor = conn.cursor()
    sql = "show tables"
    try:
        cursor.execute(sql)
        rows = cursor.fetchall()
        for row in rows:
            tabNames.append(row[0])
    except Exception as ex:
        print(f"    访问目标数据库'{scheName}'遇到错误（{str(ex)}）'，程序终止。")
        rollbackTransaction(haloConn)
        sys.exit(0)
    cursor.close()
    return tabNames


def runShowColumns(conn, scheName, tabName):
    colNames = []
    cursor = conn.cursor()
    sql = "show columns from " + tabName
    try:
        cursor.execute(sql)
        rows = cursor.fetchall()
        for row in rows:
            colNames.append(row[0])
    except Exception as ex:
        print(f"    访问目标数据库表'{scheName}.{tabName}'遇到错误（{str(ex)}）'，程序终止。")
        rollbackTransaction(haloConn)
        sys.exit(0)
    cursor.close()
    return colNames


def insertIntoSchemaTableColumns(conn, scheName, tabName, colName):
    if (scheName == "information_schema"):
        scheName = "mys_informa_schema"

    cursor = conn.cursor()
    sql = "insert into mys_informa_schema.schema_table_columns values('" + scheName + "', '" + tabName + "', '" + colName + "')"
    try:
        cursor.execute(sql)
        conn.commit();
    except Exception as ex:
        print(f"    访问数据库表'mys_informa_schema.schema_table_columns'遇到错误（{str(ex)}）'，程序终止。")
        rollbackTransaction(haloConn)
        sys.exit(0)
    cursor.close()


if __name__ == "__main__":
    checkPythonVersion();

    mysqlConn = pymysql.connect(host=mysqlHost, port=mysqlPort, user=mysqlUser, password=mysqlPassword, autocommit=True, client_flag = CLIENT.MULTI_STATEMENTS)
    haloConn = pymysql.connect(host=haloMySQLHost, port=haloMySQLPort, user=haloMySQLUser, password=haloMySQLPassword, db="mys_informa_schema", autocommit=True, client_flag = CLIENT.MULTI_STATEMENTS)

    if (businessDBs == ""):
        print("请指定目标数据库！");
        sys.exit(0)
    busDbs = businessDBs.split(",")
    if (len(busDbs) == 0):
        print("请指定目标数据库！");
        sys.exit(0)
    for busDb in busDbs:
        busDb = busDb.strip();
        if (busDb == ""):
            print(f"目标数据库设置 '{businessDBs}' 不合法！");
            sys.exit(0)

    scheNames = []
    for scheName in busDbs:
        scheName = scheName.strip();
        scheNames.append(scheName)

    print(f"开始迁移字段名称。")
    startTransaction(haloConn);

    for scheName in scheNames:
        clearSchemaTableColumns(haloConn, scheName);
        runUseDatabase(mysqlConn, scheName)
        scheName = scheName.lower()
        tabNames = []
        tabNames = runShowTables(mysqlConn, scheName)
        for tabName in tabNames:
            colNames = runShowColumns(mysqlConn, scheName, tabName)
            tabName = tabName.lower()
            for colName in colNames:
                insertIntoSchemaTableColumns(haloConn, scheName, tabName, colName)
        print(f"    完成 {scheName}")

    commitTransaction(haloConn);
    print(f"字段名称迁移完成。")

    mysqlConn.close();
    haloConn.close();

