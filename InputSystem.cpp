#include "InputSystem.h"
#include <Windows.h>

InputSystem* InputSystem::m_input_system = nullptr;

InputSystem::InputSystem()
{
}

InputSystem::~InputSystem()
{
	InputSystem::m_input_system = nullptr;
}

void InputSystem::update()
{

	// Handle mouse input
	POINT current_mouse_pos = {};
	::GetCursorPos(&current_mouse_pos);

	if (m_first_time)
	{
		m_old_mouse_pos = Point(current_mouse_pos.x, current_mouse_pos.y);
		m_first_time = false;
	}

	if (current_mouse_pos.x != m_old_mouse_pos.m_x || current_mouse_pos.y != m_old_mouse_pos.m_y)
	{
		// Mouse move event (notifies all listeners)
		std::unordered_set<InputListener*>::iterator it = m_set_listeners.begin();

		while (it != m_set_listeners.end())
		{
			(*it)->onMouseMove(Point(current_mouse_pos.x, current_mouse_pos.y));
			++it;
		}
	}
	m_old_mouse_pos = Point(current_mouse_pos.x, current_mouse_pos.y);


	if (::GetKeyboardState(m_keys_state))
	{
		for (unsigned int i = 0; i < 256; i++)
		{
			// Check if the key is down
			if (m_keys_state[i] & 0x80)
			{
				std::unordered_set<InputListener*>::iterator it = m_set_listeners.begin();

				while (it != m_set_listeners.end())
				{
					if (i == VK_LBUTTON)
					{
						if (m_keys_state[i] != m_old_keys_state[i])
							(*it)->onLeftMouseDown(Point(current_mouse_pos.x, current_mouse_pos.y));
					}
					else if (i == VK_RBUTTON)
					{
						if (m_keys_state[i] != m_old_keys_state[i])
							(*it)->onRightMouseDown(Point(current_mouse_pos.x, current_mouse_pos.y));
					}
					else
						(*it)->onKeyDown(i);

					++it;
				}
			}
			else // the key is up
			{
				if (m_keys_state[i] != m_old_keys_state[i])
				{
					std::unordered_set<InputListener*>::iterator it = m_set_listeners.begin();

					while (it != m_set_listeners.end())
					{
						if (i == VK_LBUTTON) (*it)->onLeftMouseUp(Point(current_mouse_pos.x, current_mouse_pos.y));

						else if (i == VK_RBUTTON) (*it)->onRightMouseUp(Point(current_mouse_pos.x, current_mouse_pos.y));

						else
							(*it)->onKeyUp(i);
							++it;
					}
				}

			}

		}
		// This stores current state of the keys in the old keys state buffer.
		::memcpy(m_old_keys_state, m_keys_state, sizeof(unsigned char) * 256);

	}
}

void InputSystem::addListener(InputListener* listener)
{
	m_set_listeners.insert(listener);
}

void InputSystem::removeListener(InputListener* listener)
{
	m_set_listeners.erase(listener);
}

void InputSystem::setCursorPosition(const Point& pos)
{
	::SetCursorPos(pos.m_x, pos.m_y);
}

void InputSystem::showCursor(bool show)
{
	::ShowCursor(show);
}

InputSystem* InputSystem::get()
{
	static InputSystem system;
	return &system;
}

void InputSystem::create()
{
	if (InputSystem::m_input_system)
		throw std::exception("InputSystem has already been created.");
	InputSystem::m_input_system = new InputSystem();
}

void InputSystem::release()
{
	if (!InputSystem::m_input_system)
		return;
	delete InputSystem::m_input_system;
}
