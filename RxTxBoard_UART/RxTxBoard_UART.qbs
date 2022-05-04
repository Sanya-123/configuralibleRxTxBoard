//import qbs
//import qbs.FileInfo
//// я файл с стартапом от того контролера закинул в папку 1111 а тут использовал SPI1
//Product {
//    name: "firmwareUART.elf"             // название выходного файла (без суффикса, он зависит от цели)
//    // cpp.executableSuffix: ".elf"  // но почему-то работает не всегда

//    property string hwVersion: "1"
//    property string fwVersion: "1"

//    type: ["application", "bin", "hex", "size"] // Что мы хотим получить на выходе
//    Depends { name:"cpp" }              // этот продукт зависит от компилятора C++


//    cpp.defines: [
//        "TOOLCHAIN_GCC_CW=1",
//        "STM32F100C6",
//        "STM32F10X_MD_VL",  // "STM32F10X_LD_VL",
//        "STM32F100xB",
//        //    "USE_RTOS", //If use FreeRTOS anauther only shedyler
//        "VERSION_FRIM=0x0101",    //version frim 0x0106 -> 01 version board 06 version frimware
//    ]

//    cpp.commonCompilerFlags: [
//        "-mthumb",              //стандартные инструкции
//        "-mcpu=cortex-m3",
//        "-fdata-sections",
//        "-ffunction-sections",  //на этапе линковки убирать не используемые функции
//        "-fno-inline",
////        "-std=c99",             //версия си
//        "-fshort-enums",
//        "-finput-charset=UTF-8",
//        "-fexec-charset=cp1251",
//        "-std=gnu11",
//        "-Wall",
//        "-Wpedantic",
//        "-Wno-main",
//        "-Wno-unused-function",
//        "-Wno-old-style-declaration",
//        "-specs=nano.specs",
//        "-J",
////        "-flto",//с ним размер только больше
//    ]
//    cpp.linkerFlags: [
//        "-Wl,-print-memory-usage",
////        "-flto",
//        "-mthumb",
//        "-mcpu=cortex-m3",
////        "--specs=nosys.specs",
//        "-Wl,--start-group",    //-Wl Pass опция длякомпоновшика
//        "-Wl,--gc-sections",
//        "-lnosys",
//       // "-lgcc",
//       // "-lc"
//    ]
//    cpp.includePaths: [
//        "cmsis/",
//        "cmsis_boot/",
//        "cmsis_boot/startup",
////        "Freertos/",
//        "src/",
//        "src/ADF4351/",
//        "src/uprav/",
//        "Sheduler/",
////        "WAKE/",
//    ]

//    files: [
//        "*.c",
//        "*.h",
//        "cmsis/*",
//        "cmsis_boot/*",
//        "cmsis_boot/startup/*",
//    //    "Freertos/*.c",
//    //    "Freertos/*.h",
//        "src/*",
//        "src/uprav/*",
//        "Sheduler/*",
//        "*.ld",
//    ]

//    cpp.linkerScripts: [
////    "link.ld",
////    "memory.ld"
////        "stm32.ld"
//        "stm32_2.ld"
////    "STM32F100C6_FLASH.ld"
//    ]

////    Group {//Anouther way inclide lincer scripts
////        name: "linker scripts"
////        files: ["stm32.ld"]
////        fileTags: ["linkerscript"]
////    }


//    Properties {
//        condition: qbs.buildVariant === "debug"
//        cpp.defines: outer.concat(["DEBUG=1"])
//    }

//    //For version system in outher things
//    Rule {
//        id: bin
//        inputs: "application"
//        Artifact {
//            fileTags: ["bin"]
//           filePath: FileInfo.baseName(input.filePath) + ".bin"
//        }
//        prepare: {
//                var args = ["-O", "binary", input.filePath, output.filePath];
//                var cmd = new Command("arm-none-eabi-objcopy", args);
//                cmd.description = "converting to bin: "+FileInfo.fileName(input.filePath);
//                cmd.highlight = "linker";
//                return cmd;
//        }
//    }

//    Rule {
//        id: hex
//        inputs: "application"
//        Artifact {
//            fileTags: ["hex"]
//            filePath: FileInfo.baseName(input.filePath) + ".hex"
//        }
//        prepare: {
//            var args = ["-O", "ihex", input.filePath, output.filePath];
//            var cmd = new Command("arm-none-eabi-objcopy", args);
//            cmd.description = "converting to hex: "+FileInfo.fileName(input.filePath);
//            cmd.highlight = "linker";
//            return cmd;
//        }
//    }

//    Rule {
//        id: size
//        inputs: ["application"]
//        Artifact {
//            fileTags: ["size"]
//            filePath: "-"
//        }
//        prepare: {
//            var args = [input.filePath];
//            var cmd = new Command("arm-none-eabi-size", args);
//            cmd.description = "File size: " + FileInfo.fileName(input.filePath);
//            cmd.highlight = "linker";
//            return cmd;
//        }
//    }
//}

import qbs
import qbs.FileInfo
// я файл с стартапом от того контролера закинул в папку 1111 а тут использовал SPI1
Product {
    type: ["application", "bin", "hex", "size"]// Что мы хотим получить на выходе
    Depends { name:"cpp" }// этот продукт зависит от компилятора C++
    cpp.positionIndependentCode: false
    cpp.enableExceptions: false
    cpp.executableSuffix: ".elf"

    property string hwVersion: "1"
    property string fwVersion: "1"

    cpp.defines: [
        "TOOLCHAIN_GCC_CW=1",
        "STM32F100C6",
        "STM32F10X_MD_VL",  // "STM32F10X_LD_VL",
        "STM32F100xB",
        //    "USE_RTOS", //If use FreeRTOS anauther only shedyler
        "VERSION_FRIM=0x0101",    //version frim 0x0106 -> 01 version board 06 version frimware
    ]

    cpp.includePaths: [
        "cmsis/",
        "cmsis_boot/",
        "cmsis_boot/startup",
//        "Freertos/",
        "src/",
        "src/ADF4351/",
        "src/uprav/",
        "Sheduler/",
//        "WAKE/",
    ]

    files: [
        "*.c",
        "*.h",
        "cmsis/*",
        "cmsis_boot/*",
        "cmsis_boot/startup/*",
    //    "Freertos/*.c",
    //    "Freertos/*.h",
        "src/*",
        "src/uprav/*",
        "Sheduler/*",
        "*.ld",
    ]

    cpp.libraryPaths: [
        "./"
    ]

    cpp.driverFlags: [
        "-mthumb",              //стандартные инструкции
        "-mcpu=cortex-m3",
        "-fdata-sections",
        "-ffunction-sections",  //на этапе линковки убирать не используемые функции
        "-fno-inline",
//        "-std=c99",             //версия си
        "-fshort-enums",
        "-finput-charset=UTF-8",
        "-fexec-charset=cp1251",
        "-std=gnu11",
        "-Wall",
        "-Wpedantic",
        "-Wno-main",
        "-Wno-unused-function",
        "-Wno-old-style-declaration",
        "-specs=nano.specs",
        "-J",
//        "-flto",//с ним размер только больше
    ]
    cpp.linkerFlags: [
        "-print-memory-usage",
        "-start-group",    //-Wl Pass опция длякомпоновшика
        "-gc-sections",
        "-lnosys",
        "-T" + path + "/stm32_2.ld",
    ]

//    Group {//Anouther way inclide lincer scripts
//        name: "linker scripts"
//        files: ["stm32.ld"]
//        fileTags: ["linkerscript"]
//    }


    Properties {
        condition: qbs.buildVariant === "debug"
        cpp.driverFlags: outer.concat(["-Og", "-ggdb", "-DDEBUG=1"])
    }

    Properties {
        condition: qbs.buildVariant === "release"
        cpp.driverFlags: outer.concat(["-Og", "-g0", "-s", "-DDEBUG=0"])
    }

    Rule {
        id: bin
        inputs: "application"
        Artifact {
            fileTags: ["bin"]
           filePath: FileInfo.baseName(input.filePath) + ".bin"
        }
        prepare: {
                var args = ["-O", "binary", input.filePath, output.filePath];
                var cmd = new Command("arm-none-eabi-objcopy", args);
                cmd.description = "converting to bin: "+FileInfo.fileName(input.filePath);
                cmd.highlight = "linker";
                return cmd;
        }
    }

    Rule {
        id: hex
        inputs: "application"
        Artifact {
            fileTags: ["hex"]
            filePath: FileInfo.baseName(input.filePath) + ".hex"
        }
        prepare: {
            var args = ["-O", "ihex", input.filePath, output.filePath];
            var cmd = new Command("arm-none-eabi-objcopy", args);
            cmd.description = "converting to hex: "+FileInfo.fileName(input.filePath);
            cmd.highlight = "linker";
            return cmd;
        }
    }

    Rule {
        id: size
        inputs: ["application"]
        Artifact {
            fileTags: ["size"]
            filePath: "-"
        }
        prepare: {
            var args = [input.filePath];
            var cmd = new Command("arm-none-eabi-size", args);
            cmd.description = "File size: " + FileInfo.fileName(input.filePath);
            cmd.highlight = "linker";
            return cmd;
        }
    }
}

