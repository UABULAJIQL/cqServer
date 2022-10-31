#ifndef _STATETEMPLATE_H
#define _STATETEMPLATE_H

#include <functional>
#include <map>

// 状态机基类 T为管理状态的类
template <typename enumType, class T> class StateTemplate {
  public:
    StateTemplate() {}

    void SetParentObj(T *pObj) { _pParentObj = pObj; }

    virtual ~StateTemplate() = default;
    // 获取当前的状态类型
    virtual enumType GetStae() = 0;
    // 状态机的内部更新
    virtual enumType Update() = 0;
    // 进入本状态
    virtual void EnterState() = 0;
    // 离开本状态
    virtual void LeaveState() = 0;

  protected:
    T *_pParentObj;
};

// enumType为类型 StateClass为状态机类 T为状态的管理类
template <typename enumType, class StateClass, class T> class StateTemplateMgr {
  public:
    virtual ~StateTemplateMgr() {
        if (_pState != nullptr) {
            delete _pState;
        }
    }

    // 初始化状态
    void InitStateTemplateMgr(enumType defaultState) {
        _defaultState = defaultState;
        RegisterState();
    }

    // 改变状态
    void ChangeState(enumType stateType) {

        StateClass *pNewState = CreateStateObj(stateType);
        // mpa中无该类型
        if (pNewState == nullptr) {
            return;
        }

        if (pNewState != nullptr) {

            // 停止当前状态
            if (_pState != nullptr) {
                _pState->LeaveState();
                delete _pState;
            }

            // 进入当前状态
            _pState = pNewState;
            _pState->EnterState();
        }
    }

    void UpdateState() {

        // 当前状态类为空
        if (_pState == nullptr) {
            // 设置为模式类型
            ChangeState(_defaultState);
        }

        // 状态类更新
        enumType curState = _pState->Update();

        // 如果状态类型改变了那么改变类型
        if (curState != _pState->GetState()) {
            ChangeState(curState);
        }
    }

  protected:
    virtual void RegisterState() = 0;

    // 创建状态类
  public:
    // typedef StateClass *(*CreateIstancePt)();
    using CreateIstancePt = StateClass *(*)();

    // 根据类型创建对应的状态类
    StateClass *CreateStateObj(enumType enumValue) {

        // 没找到状态类型返回空
        auto iter = _dynCreateMap.find(enumValue);
        if (iter == _dynCreateMap.end())
            return nullptr;

        // 找到调用对应的状态处理函数
        CreateIstancePt np = iter->second;
        // 更新当前状态类型
        StateClass *pState = np();

        pState->SetParentObj(static_cast<T *>(this));

        return pState;
    }

    void RegisterStateClass(enumType enumValue, CreateIstancePt np) {
        _dynCreateMap[enumValue] = np;
    }

  protected:
    // 每种状态的生成函数指针
    std::map<enumType, CreateIstancePt> _dynCreateMap;
    // 当前状态的指针
    StateClass *_pState{nullptr};
    // 最初的默认状态
    enumType _defaultState;
};

#endif
