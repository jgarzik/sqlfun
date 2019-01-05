#!/usr/bin/env node

const fs = require('fs');
const assert = require('assert');

const lines = fs.readFileSync(process.argv[2], {encoding:'utf8'}).split("\n");

function getRecordStream(lineList)
{
	var recs = [];
	const blankRx = /^\s*$/;
	lineList.forEach(function(line) {
		if (blankRx.test(line))
			return;

		try {
			const record = JSON.parse(line);
			recs.push(record);
		} catch (e) {
			console.error("Invalid line: ", line);
			console.error(e);
		}
	});

	return recs;
}

function parseRecordStream(recs)
{
	var stk = [];
	var ret = null;

	recs.forEach(function(current) {
		if (current.op && current.op == 'DEFINE-COL') {
			var objColumn = {
				name: current.name,
				flags: current.flags,
				attr: [],
			};
			while (stk.length > 0) {
				const rec = stk.pop();
				if ('attr' in rec)
					objColumn.attr.push(rec);
				else if (rec.op && rec.op == 'START-COL')
					break;
				else {
					assert.fail('unhandled rec: ' + JSON.stringify(rec));
				}
			}
			stk.push(objColumn);

		} else if (current.op && current.op == 'CREATE-TABLE') {
			var objTable = {
				verb: 'CREATE_TABLE',
				name: current.name,
				temp: current.temp,
				if_n_exists: current.if_n_exists,
				n_cols: current.n_cols,
				columns: [],
			};
			var wantCols = objTable.n_cols;
			while (wantCols-- > 0) {
				const col = stk.pop();
				assert(col !== undefined);

				objTable.columns.push(col);
			}
			stk.push(objTable);

		} else if (current.op && current.op == 'CREATE-DB') {
			var objDb = {
				verb: 'CREATE_DATABASE',
				name: current.name,
				if_ne: current.if_ne,
			};
			stk.push(objDb);

		} else if (current.op && current.op == 'SELECT') {
			var objSelect = {
				verb: 'SELECT',
				opts: current.opts,
				n_expr: current.n_expr,
				n_tbl_ref: current.n_tbl_ref,
				tables: [],
				exprs: [],
			};
			var wantTabs = objSelect.n_tbl_ref;
			while (wantTabs-- > 0) {
				const tab = stk.pop();
				assert(tab !== undefined);

				objSelect.tables.push(tab.name);
			}
			var wantExpr = objSelect.n_expr;
			while (wantExpr-- > 0) {
				const an_expr = stk.pop();
				assert(an_expr !== undefined);

				objSelect.exprs.push(an_expr);
			}
			stk.push(objSelect);

		} else if (current.op && current.op == 'STMT') {
			const stmt = stk.pop();
			assert(stk.length === 0);

			ret = stmt;
		} else if ('result' in current) {
			// EOF; do nothing

		} else {
			// console.log("PUSH", current);
			stk.push(current);
		}
	});

	return ret;
}

const recs = getRecordStream(lines);

// console.dir(recs);

const stmt = parseRecordStream(recs);

console.log(JSON.stringify(stmt, null, 2));

