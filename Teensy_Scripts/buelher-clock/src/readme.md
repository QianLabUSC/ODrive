### Departing Notes

Mechanical Design

-   motor mount, encoder mount, 
    -   CAD design aligns Magnet on Antrgravity motor axle above AS4067 Encoder
    -   magnet needs to be glued into alignment with axle
    -   note, Alex has CAD files (may want to commit `.sldpart` to repo)
    -   remaining issues: magnet easily detaches from axle, encoder seems to throw many errors
    -   Ongoing work: replacing AS4067 with mechanical [CUI encoder](https://odriverobotics.com/shop/cui-amt-102)
-   body



Motor Control

-   uses ODrive Board
-   uses brusheless DC Motors
    -   U8-II
    -   Antigravity
    -   each has their own parameters that need to be set on the board
    -   refer to parameters JSON Dump ("config backups" folder)



Code

-   buelher clock test function implemented under `buelher clock` folder (other folder is John's)
    -   only controls one motor (either axis 1 or 0)
    -   only runs for 500 seconds
    -   calls into ODrive "set position" commands, uses [unfiltered position control](https://docs.odriverobotics.com/)
-   relies on Platform IO Visual Studio Code Extension (ask John)