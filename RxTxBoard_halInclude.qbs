import qbs
import qbs.FileInfo

Properties {
    condition: true
    Depends { name:"cpp" }              // этот продукт зависит от компилятора C++
    cpp.includePaths:  outer.concat([
        "cmsis/",
        "cmsis_boot/",
        "cmsis_boot/startup",
    ])
}


