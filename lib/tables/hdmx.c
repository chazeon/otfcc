#include "support/util.h"
#include "otfcc/table/hdmx.h"

table_hdmx *table_read_hdmx(caryll_Packet packet, const otfcc_Options *options, table_maxp *maxp) {
	FOR_TABLE('hdmx', table) {
		font_file_pointer data = table.data;

		table_hdmx *hdmx;
		NEW(hdmx);
		hdmx->version = read_16u(data);
		hdmx->numRecords = read_16u(data + 2);
		hdmx->sizeDeviceRecord = read_32u(data + 4);
		NEW_N(hdmx->records, hdmx->numRecords);

		for (uint32_t i = 0; i < hdmx->numRecords; i++) {
			hdmx->records[i].pixelSize = *(data + 8 + i * (2 + maxp->numGlyphs));
			hdmx->records[i].maxWidth = *(data + 8 + i * (2 + maxp->numGlyphs) + 1);
			NEW_N(hdmx->records[i].widths, maxp->numGlyphs);
			memcpy(hdmx->records[i].widths, data + 8 + i * (2 + maxp->numGlyphs) + 2, maxp->numGlyphs);
		}

		return hdmx;
	}
	return NULL;
}

void table_delete_hdmx(table_hdmx *table) {
	if (table->records != NULL) {
		for (uint32_t i = 0; i < table->numRecords; i++) {
			if (table->records[i].widths != NULL) FREE(table->records[i].widths);
		}
		FREE(table->records);
	}
	FREE(table);
}
