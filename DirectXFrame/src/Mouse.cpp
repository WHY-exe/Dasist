#include "Mouse.h"

bool Mouse::IsEmpty() const noexcept
{
    return m_qMouseEvent.empty();
}

Mouse::Event::LRStatus Mouse::lrButtonStatus() const noexcept
{
    return m_lrStatus;
}

const POINTS& Mouse::GetMousePt() const noexcept
{
    return m_ptMouse;
}

Mouse::Event Mouse::ReadEvent() noexcept
{
    if (!m_qMouseEvent.empty())
    {
        Event e = m_qMouseEvent.front();
        m_qMouseEvent.pop();
        return e;
    }
    return Event();
}

bool Mouse::IsInWindow() const noexcept
{
    return m_bInWindow;
}

int Mouse::GetWheelDelta() const noexcept
{
    return m_wheelDeltaCarry;
}

void Mouse::ClearEventQueue() noexcept
{
    m_qMouseEvent = std::queue<Event>();
}

void Mouse::OnLButtonDown(LPARAM lParam) noexcept
{
    m_ptMouse = MAKEPOINTS(lParam);
    m_qMouseEvent.push(Event(Event::Status::LPressed, m_ptMouse, Event::LRStatus(true, false)));
    QueueControl();
}

void Mouse::OnLButtonUp(LPARAM lParam) noexcept
{
    m_ptMouse = MAKEPOINTS(lParam);;
    m_qMouseEvent.push(Event(Event::Status::LRelease, m_ptMouse, Event::LRStatus()));
    QueueControl();
}

void Mouse::OnRButtonDown(LPARAM lParam) noexcept
{
    m_ptMouse = MAKEPOINTS(lParam);;
    m_qMouseEvent.push(Event(Event::Status::RPressed, m_ptMouse, Event::LRStatus(false, true)));
    QueueControl();
}

void Mouse::OnRButtonUp(LPARAM lParam) noexcept
{
    m_ptMouse = MAKEPOINTS(lParam);
    m_qMouseEvent.push(Event(Event::Status::RRelease, m_ptMouse, Event::LRStatus()));
    QueueControl();
}

void Mouse::OnMouseMove(LPARAM lParam) noexcept
{
    m_ptMouse = MAKEPOINTS(lParam);;
    m_qMouseEvent.push(Event(Event::Status::Move, m_ptMouse, Event::LRStatus()));
    QueueControl();
}

void Mouse::OnMouseEnter() noexcept
{
    m_bInWindow = true;
    m_qMouseEvent.push(Event(Event::Status::Enter, m_ptMouse, Event::LRStatus()));
    QueueControl();
}

void Mouse::OnMouseLeave() noexcept
{
    m_bInWindow = false;
    m_qMouseEvent.push(Event(Event::Status::Leave, m_ptMouse, Event::LRStatus()));
    QueueControl();
}

void Mouse::OnMouseWheelUp() noexcept
{
    m_qMouseEvent.push(Event(Event::Status::WheelUp, m_ptMouse, Event::LRStatus()));
    QueueControl();
}

void Mouse::OnMouseWheelDown() noexcept
{
    m_qMouseEvent.push(Event(Event::Status::WheelDown, m_ptMouse, Event::LRStatus()));
    QueueControl();
}

void Mouse::OnMouseWheel(WPARAM wParam) noexcept
{
    int delta = GET_WHEEL_DELTA_WPARAM(wParam);
    m_wheelDeltaCarry += delta;
    if (m_wheelDeltaCarry >= WHEEL_DELTA)
    {
        OnMouseWheelUp();
    }
    else if(m_wheelDeltaCarry <= -WHEEL_DELTA)
    {
        OnMouseWheelDown();
    }
}

void Mouse::QueueControl() noexcept
{
    while (m_qMouseEvent.size() > m_MaxQueueSize)
    {
        m_qMouseEvent.pop();
    }
}
