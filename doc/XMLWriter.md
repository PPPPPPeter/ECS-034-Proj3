# XMLWriter

## Struct

CXMLWriter <Writer>(std::shared_ptr< CDataSink > sink);

## Functions

### WriteEntity

Usage:
<Writer>.WriteEntity(const SXMLEntity &entity);

To write a XML entity to datasink;

### Flush

Usage:
<Writer>.Flush();

To add end elements for unclosed start elements in LIFO order.
