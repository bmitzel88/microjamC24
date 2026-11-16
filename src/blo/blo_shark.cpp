#include "blo/blo_shark.h"

#include "bn_sprite_items_shark.h"

namespace blo
{
    /**
     * shark constructor
     *
     * @param starting_position the location to start the shark at
     */
    shark::shark(bn::fixed_point starting_position, int window, bool has_fakeout) : _sprite(bn::sprite_items::shark.create_sprite(starting_position)),
                                                                                    _window(window),
                                                                                    _has_fakeout(has_fakeout)
    {
    }

    void shark::update()
    {
        _frames++;
        _sprite.set_visible(isEmerged() || isFakeout());
    }

    void shark::bonk()
    {
        if (isEmerged())
        {
            _bonked = true;
        }
        else
        {
            _snack_stolen = true;
        }
    }

    bool shark::snack_stolen() const
    {
        return _snack_stolen;
    }

    /**
     * Function that returns true if the shark has been hit, false if it hasnt
     *
     */
    bool shark::bonked() const
    {
        return _bonked;
    }

    /**
     * Function that returns true if the shark has fully emerged, false if it has not
     *
     */
    bool shark::isEmerged() const
    {
        return _frames >= _emerge_start && _frames < _emerge_start + _window;
    }

    bool shark::isFakeout() const
    {
        return _has_fakeout && _frames >= _fakeout_start && _frames < _fakeout_start + _fakeout_length ;
    }

}