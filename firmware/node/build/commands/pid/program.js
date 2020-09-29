"use strict";
var __awaiter = (this && this.__awaiter) || function (thisArg, _arguments, P, generator) {
    function adopt(value) { return value instanceof P ? value : new P(function (resolve) { resolve(value); }); }
    return new (P || (P = Promise))(function (resolve, reject) {
        function fulfilled(value) { try { step(generator.next(value)); } catch (e) { reject(e); } }
        function rejected(value) { try { step(generator["throw"](value)); } catch (e) { reject(e); } }
        function step(result) { result.done ? resolve(result.value) : adopt(result.value).then(fulfilled, rejected); }
        step((generator = generator.apply(thisArg, _arguments || [])).next());
    });
};
Object.defineProperty(exports, "__esModule", { value: true });
exports.register = void 0;
const debug = require("../../log");
// Programmer for E5DC-QX2ASM-002 Omron
/**
 *
 * @link Modbus Docs https://www.npmjs.com/package/modbus-serial
 *
 * @link Serial Port CLI https://serialport.io/docs/guide-cli
 */
const defaultOptions = (yargs) => {
    return yargs.option('port', {
        default: 'COM7',
        describe: 'serial port'
    });
};
let options = (yargs) => defaultOptions(yargs);
// npm run build ; node ./build/main.js pid:program 
exports.register = (cli) => {
    return cli.command('pid:program', 'Omron PID programmer', options, (argv) => __awaiter(void 0, void 0, void 0, function* () {
        if (argv.help) {
            return;
        }
        const port = argv.port;
        /*
        const sp = new SerialPort(port,{ baudRate: 256000,parity:'even', dataBits:8 });
        parser.on('data', line => console.log(`> ${line}`))*/
        const ModbusRTU = require("modbus-serial");
        const client = new ModbusRTU();
        function end() {
            console.log('read registers');
            client.close();
        }
        function read() {
            client.setID(1);
            // on device number 1.
            console.log('read registers');
            // 01 03 00 00 00 02 C4 0B
            client.readHoldingRegisters(2, 0).then((v) => {
                console.log('v', v);
                end();
            });
        }
        function write() {
            console.log('write registers');
            // 01 03 20 00 00 01 8FCA
            // write the values 0, 0xffff to registers starting at address 0
            // on device number 1.
            // client.writeRegisters(1, [0]).then(read);
        }
        debug.info(`Connecting to ${argv.port}`);
        // open connection to a serial port
        client.connectRTU(argv.port, { baudRate: 9600, parity: 'even' }, read);
    }));
};
//# sourceMappingURL=program.js.map