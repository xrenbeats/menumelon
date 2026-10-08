#include <Geode/Geode.hpp>
#include <Geode/modify/MenuLayer.hpp>
#include <algorithm>
#include <chrono>
#include <cmath>

using namespace geode::prelude;

class MenuMelonPopup : public Popup<> {
protected:
    bool setup() override {
        this->setTitle("MenuMelon");

        auto addLine = [this](char const* text, float y) {
            auto label = CCLabelBMFont::create(text, "bigFont.fnt");
            label->setScale(0.42f);
            label->setAnchorPoint({0.f, 0.5f});
            label->setPosition({24.f, y});
            m_mainLayer->addChild(label);
        };

        addLine("Macros", 205.f);
        addLine("Recording / Play / Continue — gameplay hooks not implemented yet", 183.f);
        addLine("Macro slots: saved slot data is not implemented yet", 163.f);
        addLine("Practice helpers", 130.f);
        addLine("No-clip and automatic safe mode controls are not implemented yet", 108.f);
        addLine("Progress customization", 75.f);
        addLine("Stars, moons, diamonds, icons, and custom speed are not implemented yet", 53.f);
        addLine("Hidden-button management requires per-button tracking", 24.f);

        return true;
    }

public:
    static MenuMelonPopup* create() {
        auto ret = new MenuMelonPopup();
        if (ret && ret->init(360.f, 260.f)) {
            ret->autorelease();
            return ret;
        }
        CC_SAFE_DELETE(ret);
        return nullptr;
    }
};

class FloatingMenuIcon : public CCNode, public CCTargetedTouchDelegate {
    CCNode* m_parent = nullptr;
    CCPoint m_touchStart;
    CCPoint m_iconStart;
    std::chrono::steady_clock::time_point m_pressStart;
    bool m_dragging = false;
    bool m_movedBeforeHold = false;

    CCPoint parentPoint(CCTouch* touch) const {
        return m_parent->convertToNodeSpace(touch->getLocation());
    }

public:
    static FloatingMenuIcon* create(CCNode* parent, CCPoint position) {
        auto ret = new FloatingMenuIcon();
        if (ret && ret->init(parent, position)) {
            ret->autorelease();
            return ret;
        }
        CC_SAFE_DELETE(ret);
        return nullptr;
    }

    bool init(CCNode* parent, CCPoint position) {
        if (!CCNode::init())
            return false;

        m_parent = parent;
        auto icon = ButtonSprite::create("M");
        if (!icon)
            return false;
        icon->setScale(0.72f);
        auto iconSize = icon->getContentSize();
        this->setContentSize({iconSize.width * 0.72f, iconSize.height * 0.72f});
        this->setAnchorPoint({0.5f, 0.5f});
        icon->setPosition({this->getContentSize().width / 2.f, this->getContentSize().height / 2.f});
        this->addChild(icon);
        this->setPosition(position);
        return true;
    }

    void onEnter() override {
        CCNode::onEnter();
        CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, -128, true);
    }

    void onExit() override {
        CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this);
        CCNode::onExit();
    }

    bool ccTouchBegan(CCTouch* touch, CCEvent*) override {
        auto point = parentPoint(touch);
        if (!this->getBoundingBox().containsPoint(point))
            return false;

        m_touchStart = point;
        m_iconStart = this->getPosition();
        m_pressStart = std::chrono::steady_clock::now();
        m_dragging = false;
        m_movedBeforeHold = false;
        return true;
    }

    void ccTouchMoved(CCTouch* touch, CCEvent*) override {
        auto point = parentPoint(touch);
        auto dx = point.x - m_touchStart.x;
        auto dy = point.y - m_touchStart.y;
        auto elapsed = std::chrono::steady_clock::now() - m_pressStart;

        if (!m_dragging) {
            if (std::abs(dx) > 8.f || std::abs(dy) > 8.f) {
                if (elapsed < std::chrono::seconds(2))
                    m_movedBeforeHold = true;
                else if (!m_movedBeforeHold)
                    m_dragging = true;
            }
        }

        if (m_dragging) {
            auto size = CCDirector::sharedDirector()->getWinSize();
            auto halfWidth = this->getContentSize().width * this->getScaleX() / 2.f;
            auto halfHeight = this->getContentSize().height * this->getScaleY() / 2.f;
            auto x = std::max(halfWidth, std::min(size.width - halfWidth, m_iconStart.x + dx));
            auto y = std::max(halfHeight, std::min(size.height - halfHeight, m_iconStart.y + dy));
            this->setPosition({x, y});
        }
    }

    void ccTouchEnded(CCTouch*, CCEvent*) override {
        if (!m_dragging && !m_movedBeforeHold) {
            if (auto popup = MenuMelonPopup::create())
                popup->show();
        }
    }

    void ccTouchCancelled(CCTouch*, CCEvent*) override {
        m_dragging = false;
        m_movedBeforeHold = false;
    }
};

class $modify(MenuLayer) {
    bool init() {
        if (!MenuLayer::init())
            return false;

        auto winSize = CCDirector::sharedDirector()->getWinSize();
        auto icon = FloatingMenuIcon::create(this, {winSize.width - 34.f, winSize.height - 44.f});
        if (icon)
            this->addChild(icon, 100);

        return true;
    }
};
