"use strict";
Object.defineProperty(exports, "__esModule", { value: true });
exports.OutputFormat = exports.OutputTarget = void 0;
/////////////////////////////////////////////////////
//
//  Application types
//
var OutputTarget;
(function (OutputTarget) {
    OutputTarget["STDOUT"] = "console";
    OutputTarget["FILE"] = "file";
})(OutputTarget = exports.OutputTarget || (exports.OutputTarget = {}));
var OutputFormat;
(function (OutputFormat) {
    OutputFormat["text"] = "text";
    OutputFormat["json"] = "json";
})(OutputFormat = exports.OutputFormat || (exports.OutputFormat = {}));
//# sourceMappingURL=types.js.map