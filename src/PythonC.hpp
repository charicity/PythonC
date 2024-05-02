#pragma once
/*
to do list:
long类型编码
优化逻辑？
*/
#include <assert.h>

#include "data.pb.h"
#include "getPyobj.hpp"
#include "setPyobj.hpp"

// 它会管理解释器的运行和结束
// 用户自行 Py_Finalize() 可能会造成问题

static MatchRuleReq* getMaps();
static bool printReqInfo(const std::string& key) {
    PyObject* global_dict = PyEval_GetGlobals();  // 借入引用
    PyObject* item =
        PyDict_GetItemString(global_dict, "__ReqInfo__");  // 借入引用

    if (item == nullptr) {
        fprintf(stderr, "[Error] __ReqInfo__ is not existed.\n");
        PyErr_SetString(PyExc_RuntimeError,
                        "[Error] __ReqInfo__ is not existed.");
        return false;
    }
    if (!PyUnicode_Check(item)) {
        fprintf(stderr, "[Error] __ReqInfo__ is not a str type.\n");
        PyErr_SetString(PyExc_RuntimeError,
                        "[Error] __ReqInfo__ is not a str type.");
        return false;
    }

    char* c = nullptr;
    PyArg_Parse(item, "s", &c);
    fprintf(stdout, "Request name: %s\t GET %s:\n", c, key.c_str());

    return true;
}

// python 调用 get_pyobj(name) ，返回 PyObject
static PyObject* get_Pyobj(PyObject* self, PyObject* args) {
    char* str;

    if (!PyArg_ParseTuple(args, "s", &str)) {
        std::cerr << "[Error] Failed to parse arguments in get_Pyobj()."
                  << std::endl;
        std::cerr << "[INFO] Usage: get_Pyobj(key_name)." << std::endl;
        PyErr_SetString(PyExc_RuntimeError,
                        "[Error] Failed to parse arguments in get_Pyobj().");

        return nullptr;  // let it crash
    }

    std::string key = str;

    if (!printReqInfo(key)) {
        return nullptr;  // let it crash
    }

    MatchRuleReq* fetch = getMaps();
    if (fetch == nullptr) {
        std::cerr << "[Error] __MatchRuleReq__ is not existed." << std::endl;
        PyErr_SetString(PyExc_RuntimeError,
                        "[Error] __MatchRuleReq__ is not existed.");
        return nullptr;
    }

    auto& map_ref = (*fetch->mutable_context_map());
    auto it = map_ref.find(key);
    if (it == map_ref.end()) {
        return Py_None;  // maybemaybemaybemaybe
    }

    PyObject* object = getPyobjFromContext(&it->second);
    printf("value: ");
    PyObject_Print(object, stdout, Py_PRINT_RAW);
    puts("");

    return object;
}

// python调用 set_pyobj(PyObject, name) ，设置对应 MatchRuleReq ，无返回
static PyObject* set_Pyobj(PyObject* self, PyObject* args) {
    char* str;
    PyObject* object = nullptr;
    if (!PyArg_ParseTuple(args, "Os", &object, &str) || object == nullptr) {
        return nullptr;
    }
    std::string key = str;
    // std::cerr << "setting " << key << std::endl;

    MatchRuleReq* fetch = getMaps();
    if (fetch == nullptr) {
        std::cerr << "fetch = null when " << key << std::endl;
        return nullptr;
    }

    auto& context = (*fetch->mutable_context_map())[key];

    setFromPyObj(object, &context);

    Py_RETURN_NONE;
}

/*接口区*/

// 定义方法
static PyMethodDef PythonCMethods[] = {
    {"get_Pyobj", get_Pyobj, METH_VARARGS, "Get pyobj from C++ host by name"},
    {"set_Pyobj", set_Pyobj, METH_VARARGS, "Set pyobj to C++ host by name"},
    {NULL, NULL, 0, NULL}};

// 定义模块
struct PyModuleDef PythonC = {PyModuleDef_HEAD_INIT, "pythonc", NULL, -1,
                              PythonCMethods};

// 模块初始化函数
PyMODINIT_FUNC PyInit_PythonC(void) { return PyModule_Create(&PythonC); }

class pythoncNamespace {
   private:
    PyObject* global_;
    PyObject* local_;

   public:
    static int InterpreterState_;
    // 存放 "__PythonC_MatchRuleReq__" 这个值对应的 python str 类型的对象
    // 随着解释器的释放创建而创建、释放而释放
    pythoncNamespace() {
        if (pythoncNamespace::InterpreterState_ == InterpreterState::STOPPED) {
            std::cerr << "[Error] Interpreter was stopped." << std::endl;
            return;
        }

        if (pythoncNamespace::InterpreterState_ ==
            InterpreterState::UNINITIALIZED) {
            PyImport_AppendInittab("pythonc", &PyInit_PythonC);
            Py_Initialize();
            pythoncNamespace::InterpreterState_ = InterpreterState::RUNNING;
        }
        global_ = PyDict_New();
        local_ = PyDict_New();
    }
    ~pythoncNamespace() {
        Py_DECREF(global_);
        Py_DECREF(local_);
        // global_ = nullptr;
        // local_ = nullptr;
    }
    enum InterpreterState { UNINITIALIZED = 0, RUNNING = 1, STOPPED = 2 };
    int getInterpreterState() const noexcept {
        return pythoncNamespace::InterpreterState_;
    }
    PyObject* getGlobal() { return global_; }
    PyObject* getLocal() { return local_; }

    static void shutdownInterpreter() {
        pythoncNamespace::InterpreterState_ = InterpreterState::STOPPED;
        Py_Finalize();
    }
};
int pythoncNamespace::InterpreterState_ =
    pythoncNamespace::InterpreterState::UNINITIALIZED;

static MatchRuleReq* getMaps() {
    PyObject* global_dict = PyEval_GetGlobals();  // 这是个借入的引用，万分注意
    PyObject* item = PyDict_GetItemString(
        global_dict, "__MatchRuleReq__");  // 这也是个借入的引用

    if (item == nullptr) {
        std::cerr << "Failed to get MatchRuleReq* in globals()" << std::endl;
        PyErr_SetString(PyExc_RuntimeError,
                        "Failed to get MatchRuleReq* in globals()");
        return nullptr;
    }

    if (!PyCapsule_CheckExact(item)) {
        std::cerr << "__MatchRuleReq__ in globals() is not a capsule"
                  << std::endl;
        PyErr_SetString(PyExc_RuntimeError,
                        "__MatchRuleReq__ in globals() is not a capsule");
        return nullptr;
    }

    return static_cast<MatchRuleReq*>(PyCapsule_GetPointer(item, nullptr));
}

bool call_python(
    PyObject* code, MatchRuleReq& maps, const std::string& ReqInfo,
    pythoncNamespace space = pythoncNamespace(),
    const std::vector<std::string>& modules = std::vector<std::string>()) {
    // PyRun_SimpleString(script.c_str());
    // info 目前也没用上，主要是不知道是不是这个需求
    // 如果是的话就在 globals() 里再塞上一个

    PyObject* tmpptr = PyCapsule_New(&maps, nullptr, nullptr);
    // 还有判 nullptr 之类的~ 先验一下需求是否正确再做吧
    int fetch =
        PyDict_SetItemString(space.getGlobal(), "__MatchRuleReq__", tmpptr);
    Py_DECREF(tmpptr);

    PyObject* tmpstr = PyUnicode_FromString(ReqInfo.c_str());
    PyDict_SetItemString(space.getGlobal(), "__ReqInfo__", tmpstr);
    Py_DECREF(tmpstr);

    for (auto& mod : modules) {
        PyObject* current_module = PyImport_ImportModule(mod.c_str());
        PyDict_SetItemString(space.getGlobal(), mod.c_str(),
                             current_module);  // 需要异常处理
        Py_DECREF(current_module);
    }

    PyObject* pythonc_module = PyImport_ImportModule("pythonc");

    PyDict_SetItemString(space.getGlobal(), "pythonc",
                         pythonc_module);  // 需要异常处理
    Py_DECREF(pythonc_module);

    // 把 MatchRuleReq* 放在 globals 字典的 __PythonC_MatchRuleReq__ 键中

    if (fetch == -1) {
        std::cerr << "[Error] Failed to set item in global diction."
                  << std::endl;
        return false;
    }

    PyObject* result =
        PyEval_EvalCode(code, space.getGlobal(), space.getLocal());
    if (result == nullptr || PyErr_Occurred()) {
        // Retrieve the exception information
        PyObject *pType, *pValue, *pTraceback;
        PyErr_Fetch(&pType, &pValue, &pTraceback);

        // Convert the exception to a string
        PyObject* pStr = PyObject_Str(pValue);
        const char* errorMsg = PyUnicode_AsUTF8(pStr);

        // Output the error message
        std::cerr << "Python error: " << errorMsg << std::endl;

        // Cleanup
        Py_XDECREF(pStr);
        Py_XDECREF(pType);
        Py_XDECREF(pValue);
        Py_XDECREF(pTraceback);
    }
    Py_XDECREF(result);
    return true;
}

PyObject* compile_python(const std::string& script, const std::string& name) {
    if (pythoncNamespace::InterpreterState_ ==
        pythoncNamespace::UNINITIALIZED) {
        pythoncNamespace();
    }
    PyObject* code =
        Py_CompileStringExFlags(script.c_str(), name.c_str(), Py_file_input,
                                nullptr, 2);  // 2 是优化标签
    if (code == nullptr) {
        std::cout << "Error Compiling String" << std::endl;
        return nullptr;
    }

    return code;
}

/*******************/
// for debug
void currentRunSingle(const std::string& path) {
    MatchRuleReq tmp1;

    std::vector<std::string> v;
    v.push_back("sys");

    std::string script2 = load_file(path, false);

    tmpPlaceAllTypes(tmp1);

    // pythoncNamespace();

    auto com = compile_python(script2, "script2");
    call_python(com, tmp1, "tmp", pythoncNamespace(), v);
}