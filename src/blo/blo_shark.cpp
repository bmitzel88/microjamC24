#include "blo/blo_shark.h"

#include "bn_sprite_items_blo_shark.h"

namespace blo
{

    /**
     * shark constructor
     *
     * @param starting_position the location to start the shark at
     */
    shark::shark(bn::fixed_point starting_position, int window, bool has_fakeout) : _sprite(bn::sprite_items::blo_shark.create_sprite(starting_position)),
                                                                                    _window(window),
                                                                                    _has_fakeout(has_fakeout)
    {
        // Set the shark sprite to use the first frame in the animation
        _sprite.set_tiles(bn::sprite_items::blo_shark.tiles_item(), 0);

        // Hide shark sprite at launch (fixes bug)
        _sprite.set_visible(false);
    }

    void shark::update()
    {
        _frames++;

        // Check how long we have to pop the shark out of the water
        const int framesPerStage = 30;
        int framesSinceEmerge = _frames - _emerge_start;

        // If the emerging stage hasn't started, hide the shark
        if (framesSinceEmerge < 0)
        {
            _emerge_stage = 0;
        }
        else if (isFakeout())
        {
            // Retreat from half emerged to partially emerged.
            _emerge_stage = 1;
        }
        else
        { // Checks if the shark is set up to fake out, and if the fakeout has ended
            if (_has_fakeout && _frames >= _fakeout_start + _fakeout_length)
            {
                // After retreating, repeat the half-emerged stage.
                framesSinceEmerge -= _fakeout_length + framesPerStage;
            }

            if (framesSinceEmerge < framesPerStage)
            {
                _emerge_stage = 1;
            }
            // Stage 2 ends after 2 stages worth of frames
            else if (framesSinceEmerge < framesPerStage * 2)
            {
                _emerge_stage = 2;
            }
            // Stage 3 lasts for the hit window after the first 2 stages
            else if (framesSinceEmerge < framesPerStage * 2 + _window)
            {
                _emerge_stage = 3;
            }
            else
            {
                _emerge_stage = 0;
                _snack_stolen = true;
            }
        }

        if (_emerge_stage == 0)
        {
            // Hide shark sprite at emergence level 0
            _sprite.set_visible(false);
        }
        // Check which shark frame to show (depending on emerge stage)
        else if (_emerge_stage == 1)
        {
            // Set the shark sprite to use the first frame in the animation
            _sprite.set_tiles(bn::sprite_items::blo_shark.tiles_item(), 0);
            _sprite.set_visible(true);
        }
        else if (_emerge_stage == 2)
        {
            // Set the shark sprite to use the second frame in the animation
            _sprite.set_tiles(bn::sprite_items::blo_shark.tiles_item(), 1);
            _sprite.set_visible(true);
        }
        else
        {
            // Set the shark sprite to use the third frame in the animation
            _sprite.set_tiles(bn::sprite_items::blo_shark.tiles_item(), 2);
            _sprite.set_visible(true);
        }
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
        return _emerge_stage == 3;
    }

    /**
     * Function that returns true if a fakeout is currently needing to happen, false if not
     *
     */
    bool shark::isFakeout() const
    {
        return _has_fakeout && _frames >= _fakeout_start && _frames < _fakeout_start + _fakeout_length;
    }
}