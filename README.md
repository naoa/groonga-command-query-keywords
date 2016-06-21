# Groonga query_keywords command plugin

クエリー構文からキーワードを抽出するコマンド

## Usage

```
plugin_register commands/query_keywords
[[0,0.0,0.0],true]
query_keywords "hoge + test OR or - NOT (kakko + kakko2)"
[[0,0.0,0.0],["kakko2","kakko","or","test","hoge"]]
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
