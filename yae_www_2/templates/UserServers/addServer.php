<? $this->render("UserServers/submenu"); ?>

<? $addForm->render(); ?>
<p class="infoMessage expand"><a href="#" class="infoMessage">Why is rcon password needed?</a></p>
<p style="width:300px;margin:0 auto;text-align:justify;display:none;">Yae needs access to the server to obtain data which will provide something to identify users with. Unfortunately, the status data available publicly only consists of nicks and xp amounts, which is not enough for any kind of identification. Thus, yae uses rcon console to invoke few commands in order to obtain more data. The commands are: pb_sv_msgprefix, pb_sv_plist, sl_sv_listplayers, status, sv_hostname and cheaters.<br />If you will change your rcon password later, yae will stop receiving data, and it will only rarely check whether the password still differs.<br />You will be able to disable server checks instantly at any time using this website.<br />Please note that running yae uses much more resources than it would take to run a few public servers of ET.</p>