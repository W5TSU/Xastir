# README
------------------------------------------------------------------------
This is a fork of the Github.com/xastir/Xastir repository.

Having worked on the code long ago I've decided to revisit and do some
updates I'd like to have.  Starting with adding support for Google Maps.

See RELEASE-NOTES-2.3.1.md for details.

W5TSU

------------------------------------------------------------------------

 Please at least SKIM this document before asking questions. In fact,
 READ IT if you've never successfully set up Xastir before. PLEASE!
 READ IT! If you haven't read this file, and ask for help
 expect to be told to READ the README file first! or RTFM :)

 Contents:

 0.    Important notice
 1.    What is Xastir?
 2.    How do I get Xastir & Git usage
 3.    Quick startup
 4.    Upgrading
 5.    Identification notes
 6.    OS-specific notes
 7.    Gating weather alerts
 8.    Boring legal stuff
 9.    Mailing list
 10.   Documentation
 11.   Obtaining help

------------------------------------------------------------------------

0. NOTICE

   Please read this file carefully before trying to set up Xastir.
   This software was developed to be used by licensed amateur radio
   operators.  You are responsible for any information transmitted
   or propagated on any network.

1. WHAT IS XASTIR?

   Xastir is an open-source project to create a free X11 graphical
   APRS(tm) client. APRS(tm) use amateur radio and Internet services to
   convey GPS mapping, weather, and positional data in a graphical
   application.  It has been developed by and for amateur radio
   enthusiasts to provide real-time data in an easy to use package.

   Xastir currently runs under several flavors of Linux and BSD Unix.
   A few people are running Xastir on Solaris Unix, FreeBSD, Lindows
   and Mac OS X, but there may be small changes necessary in order to
   get Xastir to configure/compile on some systems.  There are a few
   notes below which may help in this task.  Most of the developers use
   Linux which makes it the best supported platform at the moment.

   Xastir is an open-source project: Most sources, documentation, and
   binaries are available under the GPL license, with a few modules
   available under other open-source or public domain licenses.

   More information on Xastir can be found here:

   * http://xastir.org
   * http://github.com/Xastir

   including the latest releases, Git access (lets you
   download the latest developers' code), and information on how to join
   Xastir mailing lists.  Note that you must be subscribed in order to
   post to the mailing lists.

   SmartBeaconing(tm) was invented by Tony Arnerich (KD7TA) and Steve
   Bragg (KA9MVA) for the HamHUD project.  They offer the algorithm to
   other authors as long as proper credit is given and the term
   SmartBeaconing(tm) is used.  Thanks to Tony and Steve for that
   contribution!

   -- The Xastir Group.

2. HOW TO GET XASTIR

   Xastir is currently developed at
   <http://github.com/Xastir/Xastir>
   You can get the latest version of Xastir from there.

   You might try <http://xastir.org> for help and information,
   particularly the Xastir mailing list (listed near the bottom
   of the page).

   * Git USAGE

     Obtain the *very latest* version of Xastir under development by
     using Git. 

     See the file [README.GIT](README.GIT) for more details.

   * Release version tarballs

     You can get the latest packaged release source code without git
     at https://github.com/Xastir/Xastir/releases.  Be warned that packaged
     source tarballs may be quite old and not representative of the current
     state of the project.  We highly recommend not using this method unless
     you have a specific reason to stick to official releases.

3. QUICK STARTUP

   See [INSTALL.md](INSTALL.md) for a
   relatively quick overview of how to build and use Xastir.

   Check the Xastir wiki (http://xastir.org) for OS-specific guidance
   for building Xastir on your system.

   WINDOWS USERS:  Please refer to the [README.CYGWIN](README.CYGWIN) file for
   specific instructions.

4. UPGRADING

   Upgrading Xastir that has been built from a recent Git clone
   is as simple as running "git pull" in the source tree and
   recompiling.

5. IDENTIFICATION NOTES

   Packet radio modes, by their very nature, typically identify
   themselves with every transmission. Xastir has a few features
   targeted to people who used Xastir in demonstrations and other
   broadcasts where Xastir itself is used over radio.

   Xastir can auto-ID via voice if Festival is compiled in and/or via a
   message splashed across the screen. It does this identification
   every 9.5 minutes if enabled. These identification modes were
   designed for broadcasting Xastir across fast-scan television (for
   events perhaps). Set the "ATV_SCREEN_ID" variable to 1 to enable the
   screen message, and "SPEAK_ID" variable to 1 to enable festival to
   speak the message. These variables are in the
   ~/.xastir/config/xastir.cnf file.

6. OS SPECIFIC NOTES

   [The OS-specific notes that were here were horribly outdated
   and not maintained.  That text has been removed.  Please
   see the Xastir wiki at http://xastir.org in the "Installation Notes"
   section for OS-specific build guidance.]

7. GATING WEATHER ALERTS, STATIONS, OBJECTS/ITEMS TO RF

   ## Gating NWS Weather Alerts to RF:
 
   If you wish to gate NWS weather alerts from the Internet onto RF, you'll
   need to create a text file in the users directory as 
   ~/.xastir/data/nws-stations.txt
   List each NWS station that you would like to transmit via RF. Wildcards
   are implied for lengths of 3 or greater. Here's what an example file
   looks like:

       #
       # Seattle, WA
       SEANPW
       #
       # Portland, OR (any alert type)
       PDX
       #
       # Pendleton, OR
       PDTNPW
       #
       # Medford, OR
       MFRNPW
       #

   All text should start at the beginning of the line.

   Once that file is in place, you'll need to hook up to at least one
   Internet server that is feeding you the weather alerts. You'll also need
   to have at least one RF interface up and running with transmit enabled on
   that interface. Make sure that "Interfaces->Disable Transmit: All" is not
   selected.  You should now be gating NWS weather messages to RF.

   Turn on igate logging and look at that log file to view what you're
   sending out via RF. Don't forget to turn off logging or set up
   auto-rollover of the log files, else your hard drive might fill up with
   logging info. Auto-rollover of log files is typically accomplished via
   CRON.

   ## Gating Stations, Objects/Items to RF:

   The latest code also allows gating packets from specific stations to RF
   using the above method (except object/item packets).  You can also gate
   objects/items to RF by name.  The same wildcarding rules apply as listed
   above.  Callsigns or object/item names listed in this file are
   case-insensitive, so they'll match any case in received packets.

   Bob Bruninga, WB4APR, recommends gating these calls to RF:
        
    SCOUTS, SATERN, KIDS, REDCROSS, FOUR-H, YOUTH, GUARD, MARS, JOTA

   See his link: "Generic Callsigns for National Events" off this web page
   for his current list of recommended callsigns:

  http://www.aprs.org/aprs-jota.txt


8. BORING LEGAL STUFF

   Xastir is Copyright � by Frank Giannandrea. Xastir is
   distributed according to the GNU General Public
   License. There should be a copy of this license in the
   file COPYING. If not, write to the Free Software
   Foundation, Inc., 675 Mass Ave, Cambridge, MA
   02139, USA.

   As of Xastir 0.4.0 all changes made by the Xastir
   development team to the Xastir source code and any related
   files are Copyright (C) 2000-2023 The Xastir Group. The source
   code will still be distributed according to the GNU General
   Public License as Frank Giannandrea did in the past.

   There is no warranty, implied or whatever. You use this
   software at your own risk, no matter what purpose you put
   it to.

   You didn't pay for it, so don't expect magic.


9. MAILING LIST

   There are currently a couple of mailing lists about Xastir.
   xastir@xastir.org is the one relevant for most users.

   The xastir@xastir.org mail-list is dedicated to Bug
   reports, technical questions, your thoughts or
   suggestions on new features being added to Xastir, things
   that should be removed or fixed, amazing problems that even
   stump the guru's, etc... are what we want to see here.  You
   must be subscribed to the list in order to post messages.

   To subscribe to the Xastir mailing list, send email to:
   xastir-request@xastir.org In the body of the message,
   put "subscribe xastir"; or go to
   http://xastir.org and click on "XASTIR MAILING LISTS" (in the
   "Resources" section near the bottom) to subscribe.

        ### DO NOT SEND FRANK EMAIL ABOUT XASTIR ###

   Frank is no longer developing the Xastir code (although
   he does put a word in every now and then) so don't bother
   e-mailing him. If you have a serious problem, email the
   Xastir mailing list and it will get to the coders.

   Please, before posting to this list, see what things are
   like, and when you do post, read over your post for
   readability, spelling, and grammar mistakes. Obviously,
   we're all human (or are we?) and we all make mistakes (heck,
   look at this document! ;).

   Open discussion and debate is integral to change and
   progress. Don't flame others over mere form (grammar and
   spelling), or even substantive issues either for that
   matter. Please read and follow the mailing list rules.

   A second mailing list, xastir-dev@xastir.org is intended for
   developer's discussion.

10. DOCUMENTATION

    We're trying to get the documentation up to date. If you
    feel that anything is missing here, or that anything should
    be added etc, please email xastir@xastir.org about it,
    thank you.

11. OBTAINING HELP

    Please read the file FAQ, and make sure you've followed any relevant
    instructions in INSTALL. If the problem still exists, feel free to
    ask on the Xastir mailing-list, as described above.


  ------------------------------------------------------------------------
APRS(tm) is a Trademark of Bob Bruninga

Copyright (C) 1999 Frank Giannandrea
Copyright (C) 2000-2023 The Xastir Group
Copyright (C) 2025 Mark Grennan (W5TSU)

