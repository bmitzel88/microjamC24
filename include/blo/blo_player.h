#pragma once
namespace blo {

    class shark;


    class player {
        public:
            /**
             * player constructor
             */
            player();

            void update(shark& target);
    };

}
