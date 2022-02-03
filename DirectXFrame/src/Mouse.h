#pragma once
#include "InitWin.h"
#include <queue>
class Mouse
{
	friend class Window;
public:
	struct Event
	{
		struct LRStatus
		{
			bool m_LIsPressed = false;
			bool m_RIsPressed = false;
			LRStatus() = default;
			LRStatus(bool LIsPressed, bool RIsPressed)
				:
				m_LIsPressed(LIsPressed),
				m_RIsPressed(RIsPressed)
			{};
		};
		enum class Status
		{
			LPressed,
			LRelease,
			RPressed,
			RRelease,
			WheelUp,
			WheelDown,
			Enter,
			Leave,
			Move,
			Invalid
		};
		Status m_status;
		POINTS m_ptMouse;
		LRStatus m_lrStatus;
		
		Event()
			:
			m_status(Status::Invalid),
			m_ptMouse({ 0, 0 })
		{};
		Event(Status status, POINTS pt, LRStatus lrStatus)
			:
			m_status(status),
			m_ptMouse(pt),
			m_lrStatus(lrStatus)
		{};
	};
public:
	bool IsEmpty() const noexcept;
	Event::LRStatus lrButtonStatus() const noexcept;
	const POINTS& GetMousePt() const noexcept;
	Mouse::Event ReadEvent() noexcept;
	bool IsInWindow() const noexcept;
	int GetWheelDelta() const noexcept;
public:
	void ClearEventQueue() noexcept;
private:
	void OnLButtonDown(LPARAM lParam) noexcept;
	void OnLButtonUp(LPARAM lParam) noexcept;
	void OnRButtonDown(LPARAM lParam) noexcept;
	void OnRButtonUp(LPARAM lParam) noexcept;
	void OnMouseMove(LPARAM lParam) noexcept;
	void OnMouseEnter() noexcept;
	void OnMouseLeave() noexcept;
	void OnMouseWheelUp() noexcept;
	void OnMouseWheelDown() noexcept;
	void OnMouseWheel(WPARAM wParam) noexcept;
	void QueueControl() noexcept;
private:
	static constexpr int m_MaxQueueSize = 16;
	int m_wheelDeltaCarry = 0;
	POINTS m_ptMouse;
	Event::LRStatus m_lrStatus;
	bool m_bInWindow;
	std::queue<Event> m_qMouseEvent;
};

