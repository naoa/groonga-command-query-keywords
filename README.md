# Groonga query_keywords command plugin

クエリー構文からキーワードを抽出するコマンド

## Usage

```
plugin_register commands/query_keywords
[[0,0.0,0.0],true]
table_create Entries TABLE_NO_KEY
[[0,0.0,0.0],true]
column_create Entries title COLUMN_SCALAR ShortText
[[0,0.0,0.0],true]
query_keywords Entries "hoge + test OR or - NOT (kakko + kakko2) + title:@'huga'"
[[0,0.0,0.0],["'huga'","kakko2","kakko","or","test","hoge"]]
```

## Install

Install libgroonga-dev.

Build this command.

    % sh autogen.sh
    % ./configure
    % make
    % sudo make install

## License

Public domain. You can copy and modify this project freely.
