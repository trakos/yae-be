<?php
	class Model_News
	{
		public static function getAllNewses()
		{
			return array
			(
				array
				(
					'title' => "Stats!",
					'date'	=> "10.08.2011",
					'text'	=> "<p>During a past weeks, entire website had been rewritten from scratch. It was done mostly to allow new changes, but for now not much have changes. As a little surface change of a big underground changes, statistics were being added in a few places. I find them quite fun to browse through.</p>"
				),
				array
				(
					'title' => "YAE alive!",
					'date'	=> "05.05.2011",
					'text'	=> "<p>After a half a year of downtime (without few days) YAE comes back. For now it does it without core features: registration and update. However, the search within past results works fine.</p><p>Time will show whether it came back it in vain.</p>"
				),
				array
				(
					'title'	=> "YAE down!",
					'date'	=> "16.11.2010",
					'text'	=> "<p>Due to a problems with crossfire and registering accounts, today yae is being closed. Sorry!</p>"
				),
				array
				(
					'title' => "Fancier registration",
					'date'	=> "03.11.2010",
					'text'	=> "<p>Recently, while planning one of my crossfire-related mini-projects I've decided to make some fancier registration. Since it was similar in design to what yae has, I've decided to implement it here, as well. It's one of those little additions that are not really standing out, but all those polished details counts :)</p><br /><p>In order to see the result click the 5th link in the menu - Create account or change password.</p><br /><p>By the way - shoutout to KLX for favicon. It took me some time but somehow I recalled it today ;)"
				),
				array
				(
					'title' => "Small, but constant changes?",
					'date'	=> "20.10.2010",
					'text'	=> "<p>Today I had polished just a little bit the updater engine, for example I finally got rid of bug with bots on server ;) I'll keep an eye on an error log, my aim now is to make sure no data is dropped due to parsing problems!</p><p>On a sidenote, probably a more interesting change, though one that was really easy to one - the slacid/slacnick are now searchable.</p><p>I wonder to myself, is YAE out of beta? ;) Or is it still a project without any future?</p>"
				),
				array
				(
					'title' => "SLAC",
					'date'	=> "06.10.2010",
					'text'	=> "<p>You probably already noticed the slacid and slacnick columns that were added. This update adds quite expected functionality - you can know add server with SLAC addon. Additionally, the updater itself got a little more stable, and etpro/punkbuster obviously became optional. Hope you likes it.</p><p>NOTE: it's still an experimental version ;)</p>"
				),
				array
				(
					'title' => "Long time no changes",
					'date'	=> "20.08.2010",
					'text'	=> "<p>So, the anniversary of yae hit few days ago, and there are still some people visiting it from time to time. Surprising. So, I took my time, and finally added a real registration, with automated crossfire linking. Additionally, the page is finally visible even under index.php. By the way: thanks to qmm for keeping it alive back then when it was initially blocked ;)</p><p>Ok, now, here's the deal - if I will see people registering and adding servers, I will add more features. Feel free to send me requests at crossfire ;) I know about few things I have to do, like password reminders and sending auto-pm at crossfire when rcon password becomes outdated - but those aren't really creative :)</p>"
				),
				array
				(
					'title'	=>	"Small update",
					'date'	=>	"28.08.2009",
					'text'	=>	"<p>Today's changelog:<ul><li>human-friendly date formatting</li><li>server names</li><li>links to search out of entries in details table with player's data</li></ul></p>"
				),
				array
				(
					'title'	=>	"Changes...",
					'date'	=>	"27.08.2009",
					'text'	=>	"<p>Finally some of the changes which will be needed to push yae out of the beta stage! <ul><li>Most importantly, you can now search nicks without including color tags in it! In most cases %nick% should find the person if he's not hiding under some fake.</li><li>Secondly, I've added paging, the limit is 50 entries, after that the output is splitted into pages. </li><li>Youngest octet of ip is now hidden</li><li>That also means that you can now use wildcards for ip adresses, i.e. you can search for '79.46' or '88.14.201.*'</li><li>OS is now shown in the table with details of player. All entries before now (27.08.2009 4:30) have windows system by default, sorry ;) First linux example to be found by yae is: <a href='http://trakos.pl/yae/index.php?show=player&id=4236'>link</a>! :)</li><li>just by the way - there is an irc channel - #yae @ quakenet.</li></ul></p>"
				),
				array
				(
					'title'	=>	"Closed beta is starting!",
					'date'	=>	"16.08.2009",
					'text'	=>	"<p>From now on yae is starting logging data! Everything is in the pre-beta stage so don't expect much :) The reason the beta is closed is because it is quite probable that still some serious errors are in the code and therefore instensive debugging output is enabled. That means that higher load could cause performance problems, and additionally finding anything in the output would became impossible. Therefore for now we need small load to test everything with it! But, fear not, we should expect opening quite soon!</p>"
				)
			);
		}
	}	