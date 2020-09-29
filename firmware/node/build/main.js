"use strict";
Object.defineProperty(exports, "__esModule", { value: true });
const _cli_1 = require("./_cli");
_cli_1.defaults();
const cli = require("yargs");
const program_1 = require("./commands/pid/program");
program_1.register(cli);
var ModbusRTU = require("modbus-serial");
const argv = cli.argv;
if (argv.h || argv.help) {
    cli.showHelp();
    process.exit();
}
else if (argv.v || argv.version) {
    // tslint:disable-next-line:no-var-requires
    var client = new ModbusRTU();
    process.exit();
}
//# sourceMappingURL=main.js.map