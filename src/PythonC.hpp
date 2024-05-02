#pragma once
#include <assert.h>

#include "data.pb.h"
#include "getPyobj.hpp"
#include "setPyobj.hpp"

#define setErrorCombo(s)              \
    fprintf(stderr, s, __FUNCTION__); \
    PyErr_Format(PyExc_RuntimeError, s, __FUNCTION__)

// 它会管理解释器的运行和结束
// 用户自行 Py_Finalize() 可能会造成问题

static MatchRuleReq* getMaps();
static bool printReqInfo(const std::string& key) {
    PyObject* global_dict = PyEval_GetGlobals();  // 借入引用
    PyObject* item =
        PyDict_GetItemString(global_dict, "__ReqInfo__");  // 借入引用

    if (item == nullptr) {
        setErrorCombo("[Error] __ReqInfo__ is not existed in %s().\n");
        return false;
    }
    if (!PyUnicode_Check(item)) {
        setErrorCombo("[Error] __ReqInfo__ is not a str type in %s().\n");
        return false;
    }

    char* request_name = nullptr;

    // 官方说可以用 PyArg_Parse ，但是这个接口比较老
    if (!PyArg_Parse(item, "s", &request_name)) {
        setErrorCombo("[Error] Failed to parse __ReqInfo__ in %s().\n");
        return false;
    }

    // 打印 request 信息
    fprintf(stdout, "Request name: %s\t GET %s:\n", request_name, key.c_str());

    return true;
}

// python 调用 get_pyobj(name) ，返回 PyObject
static PyObject* get_Pyobj(PyObject* self, PyObject* args) {
    char* key_str;
    if (!PyArg_ParseTuple(args, "s", &key_str)) {
        setErrorCombo("[Error] Failed to parse key in %s().\n");
        fprintf(stderr, "[INFO] Usage: get_Pyobj(key_str).\n");

        return nullptr;  // let it crash
    }
    std::string key_string = key_str;

    if (!printReqInfo(key_string)) {
        // error log 已经在 printReqInfo 中被打印了
        return nullptr;  // let it crash
    }

    MatchRuleReq* fetch = getMaps();
    if (fetch == nullptr) {
        setErrorCombo("[Error] __MatchRuleReq__ is not existed in %s().\n");
        return nullptr;
    }

    auto& map_ref = (*fetch->mutable_context_map());  // 这个不会有错了...
    auto it = map_ref.find(key_string);
    if (it == map_ref.end()) {
        fprintf(stdout,
                "[INFO] Tried to get a not existing key, returns "
                "PY_NONE.\n");
        return Py_None;
    }

    PyObject* object = getPyobjFromContext(&it->second);

    // 输出获得的值
    fprintf(stdout, "value: ");
    PyObject_Print(object, stdout, Py_PRINT_RAW);
    puts("");

    return object;
}

// python调用 set_pyobj(PyObject, name) ，设置对应 MatchRuleReq ，无返回
static PyObject* set_Pyobj(PyObject* self, PyObject* args) {
    char* str;
    PyObject* object = nullptr;
    if (!PyArg_ParseTuple(args, "Os", &object, &str)) {
        setErrorCombo("[Error] Failed to parse arguments in %s().\n");
        fprintf(stderr, "[INFO] Usage: set_Pyobj(py_val, key_str).\n");
        return nullptr;
    }
    std::string key = str;
    // std::cerr << "setting " << key << std::endl;

    MatchRuleReq* fetch = getMaps();
    if (fetch == nullptr) {
        setErrorCombo("[Error] __MatchRuleReq__ is not existed in %s().\n");
        return nullptr;
    }

    auto& context = (*fetch->mutable_context_map())[key];

    setFromPyObj(object, &context);

    Py_RETURN_NONE;
}

/*接口区*/
std::mutex mtx;  // 保证 GIL 归属，原生 cpython 的方法在我这里跑不通...
// 看看有没有这个需求，有的话就继续封装一下，目前只是示例

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
    // PyThreadState* myInterpreter = nullptr;

   public:
    static int InterpreterState_;
    // 存放 "__PythonC_MatchRuleReq__" 这个值对应的 python str 类型的对象
    // 随着解释器的释放创建而创建、释放而释放
    pythoncNamespace() {
        // myInterpreter = Py_NewInterpreter();
        // 已经有已导入的 mypythonc 模块，未经验证

        if (pythoncNamespace::InterpreterState_ == InterpreterState::STOPPED) {
            fprintf(stderr, "[Error] Interpreter was stopped.\n");
            PyErr_Format(PyExc_RuntimeError,
                         "[Error] Interpreter was stopped.\n");
            return;
        }
        if (pythoncNamespace::InterpreterState_ ==
            InterpreterState::UNINITIALIZED) {
            fprintf(stderr, "[Error] Interpreter is uninitialized.\n");
            PyErr_Format(PyExc_RuntimeError,
                         "[Error] Interpreter is uninitialized.\n");
            return;
        }

        // if (myInterpreter == nullptr) {
        //     fprintf(stderr, "[Error] failed to get Py_NewInterpreter.\n");
        //     return;
        // }

        global_ = PyDict_New();
        local_ = PyDict_New();

        if (global_ == nullptr || local_ == nullptr) {
            setErrorCombo(
                "[Error] Failed to initialize globals() or locals() in "
                "%s().\n");
        }
    }
    ~pythoncNamespace() {
        // if (myInterpreter) Py_EndInterpreter(myInterpreter);
        if (global_) Py_DECREF(global_);
        if (local_) Py_DECREF(local_);
        // global_ = nullptr;
        // local_ = nullptr;
        // myInterpreter = nullptr;
    }

    static void init_python() {
        std::lock_guard<std::mutex> lock(mtx);

        if (pythoncNamespace::InterpreterState_ ==
            InterpreterState::UNINITIALIZED) {
            PyImport_AppendInittab("pythonc", &PyInit_PythonC);
            Py_Initialize();
            pythoncNamespace::InterpreterState_ = InterpreterState::RUNNING;
        } else {
            fprintf(stderr, "Already initialized!\n");
        }
    }

    enum InterpreterState { UNINITIALIZED = 0, RUNNING = 1, STOPPED = 2 };
    int getInterpreterState() const noexcept {
        return pythoncNamespace::InterpreterState_;
    }
    PyObject* getGlobal() { return global_; }
    PyObject* getLocal() { return local_; }

    static void shutdownInterpreter() {
        std::lock_guard<std::mutex> lock(mtx);
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

    if (item == nullptr || global_dict == nullptr) {
        setErrorCombo("[ERROR] Failed to get __MatchRuleReq__ in %s().\n");
        return nullptr;
    }

    if (!PyCapsule_CheckExact(item)) {
        setErrorCombo("[ERROR] __MatchRuleReq__ is not a capsule in %s().\n");
        return nullptr;
    }

    return static_cast<MatchRuleReq*>(PyCapsule_GetPointer(item, nullptr));
}

bool call_python(
    PyObject* code, MatchRuleReq& maps, const std::string& ReqInfo,
    pythoncNamespace space = pythoncNamespace(),
    const std::vector<std::string>& modules = std::vector<std::string>()) {
    std::lock_guard<std::mutex> lg(mtx);
    /*将 maps 放在 global 中*/
    {
        PyObject* tmpptr = PyCapsule_New(&maps, nullptr, nullptr);

        if (tmpptr == NULL) {
            setErrorCombo("[ERROR] Failed to create the capsule in %s().\n");
            return false;
        }

        if (PyDict_SetItemString(space.getGlobal(), "__MatchRuleReq__",
                                 tmpptr) == -1) {
            setErrorCombo("[ERROR] Failed to set __MatchRuleReq__ in %s().\n");
            Py_DECREF(tmpptr);
            return false;
        }
        Py_DECREF(tmpptr);

        //////////////////////

        PyObject* tmpstr = PyUnicode_FromString(ReqInfo.c_str());
        if (PyDict_SetItemString(space.getGlobal(), "__ReqInfo__", tmpstr) ==
            -1) {
            setErrorCombo("[ERROR] Failed to set __ReqInfo__ in %s().\n");
            Py_DECREF(tmpstr);
            return false;
        }
        Py_DECREF(tmpstr);
    }

    /*导入其它模块*/
    {
        for (auto& mod : modules) {
            PyObject* current_module = PyImport_ImportModule(mod.c_str());
            if (PyDict_SetItemString(space.getGlobal(), mod.c_str(),
                                     current_module) == -1) {
                fprintf(stderr, "[ERROR] Failed to import %s in %s().\n",
                        mod.c_str(), __FUNCTION__);

                PyErr_Format(PyExc_RuntimeError,
                             "[ERROR] Failed to import %s in %s().\n",
                             mod.c_str(), __FUNCTION__);
                Py_DECREF(current_module);
                return false;
            }
            Py_DECREF(current_module);
        }
    }

    /*导入pythonc模块*/
    // 主要是为了和之前写的 testcases 兼容来着
    {
        PyObject* pythonc_module = PyImport_ImportModule("pythonc");

        if (PyDict_SetItemString(space.getGlobal(), "pythonc",
                                 pythonc_module) == -1) {
            fprintf(stderr, "[ERROR] Failed to import %s in %s().\n", "pythonc",
                    __FUNCTION__);

            PyErr_Format(PyExc_RuntimeError,
                         "[ERROR] Failed to import %s in %s().\n", "pythonc",
                         __FUNCTION__);
            Py_DECREF(pythonc_module);
            return false;
        }
        Py_DECREF(pythonc_module);
    }

    /*执行，获取返回结果*/
    {
        // PyGILState_STATE gstate = PyGILState_Ensure();

        PyObject* result =
            PyEval_EvalCode(code, space.getGlobal(), space.getLocal());

        if (result == nullptr || PyErr_Occurred()) {
            PyObject *pType, *pValue, *pTraceback;
            PyErr_Fetch(&pType, &pValue, &pTraceback);

            PyObject* pStr = PyObject_Str(pValue);
            const char* errorMsg = PyUnicode_AsUTF8(pStr);

            fprintf(stderr, "Python error: %s\n", errorMsg);

            Py_XDECREF(pStr);
            Py_XDECREF(pType);
            Py_XDECREF(pValue);
            Py_XDECREF(pTraceback);
            // PyGILState_Release(gstate);
            return false;
        }
        // PyGILState_Release(gstate);
        Py_XDECREF(result);
    }

    return true;
}

PyObject* compile_python(const std::string& script, const std::string& name) {
    PyObject* code = nullptr;
    {
        std::lock_guard<std::mutex> lg(mtx);
        code = Py_CompileString(script.c_str(), name.c_str(),
                                Py_file_input);  // 2 是优化标签
    }

    if (code == nullptr) {
        fprintf(stderr, "[ERROR] Failed to compiling in %s().\n", __FUNCTION__);
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