# DSVWriter

## Struct

CDSVWriter <Writer>(std::shared_ptr< CDataSink > sink, char delimiter, bool quoteall = false);

## Functions

### WriteRow
Usage:
    <Writer>.WriteRow(const std::vector<std::string> &row);

To write a row of data to datasink, seperate data by delimiter and add double quotes if needed or specified in quoteall;