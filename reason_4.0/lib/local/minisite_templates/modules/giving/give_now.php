<?php
	reason_include_once( 'minisite_templates/modules/default.php' );
	
	$GLOBALS[ '_module_class_names' ][ 'giving/'.basename( __FILE__, '.php' ) ] = 'GiveNowModule';
	
	class GiveNowModule extends DefaultMinisiteModule
	{
		function init( $args = array() )
		{
			
		}

		function has_content()
		{
			return true;
		}

		function run()
		{

			?>
 
 	<div class="give-now">

	<div class="give-now-text">
  	
	<div class="give-now-form equal-height">
		<h2>Make a <strong>Difference</strong> Today</h2>
		  		
		<div id="form">					
		<div id="discoLinear" class="thorTable">
										
		<form class="startGift" action="/giving/givenow/" method="post" enctype="application/x-www-form-urlencoded">
		
			<div class="formElement" id="giftamountItem">
				<div class="words">
					<span class="labelText">Gift Amount</span>
				</div>
			<div class="element">
				<span class="currency">$</span><input type="text" placeholder="Enter amount" name="gift_amount" value="" size="50" maxlength="256" id="gift_amountElement" class="text">
			</div>
			</div>	
		
			<div class="formElement">
			<div class="element">	
				<div id="installment_type_container" class="radioButtons">
				<table border="0" cellpadding="1" cellspacing="0">
					<tr>
						<td valign="top"><input type="radio" id="radio_installment_type_1" name="installment_type" value="Monthly" /></td>
						<td valign="top"><label for="radio_installment_type_1">Every month</label></td>
					</tr>
					<tr>
						<td valign="top"><input type="radio" id="radio_installment_type_2" name="installment_type" value="Quarterly" /></td>
						<td valign="top"><label for="radio_installment_type_2">Every quarter</label></td>
					</tr>
					<tr>
						<td valign="top"><input type="radio" id="radio_installment_type_0" name="installment_type" value="Onetime" /></td>
						<td valign="top"><label for="radio_installment_type_0">One time</label></td>
					</tr>
				</table>
			</div>
			</div>
			</div>

			<div class="submitSection">
				<input type="submit" alt="Next Step" class="submit" value="Next Step">
			</div>
		</form>
					
		</div>
		</div>
		</div>

		<div class="other-ways equal-height">
			<h3><a href="#">Other Ways to Give</a></h3>
			<ul>
				<li>Phone</li>
				<li>Mail</li>
				<li>Electronic Funds Transfer</li>
				<li>Stock Transfer</li>
				<li>Payroll Deduction</li>
				<li>Employer Matching Gifts</li>
				<li>Planned Gifts</li>
			</ul>

			<h3><a href="#">Gift Types / Areas of Support</a></h3>
	  		<ul>
	  			<li>Annual Fund Gifts</li>
	  			<li>Reunion Gifts</li>
	  			<li>Senior Giving Campaign</li>
	  			<li>Planned Gifts</li>
	  			<li>Memorial/Honorary Gifts</li>
	  			<li>Norse Athletic Association Memberships</li>
	  			<li>Endowment/Scholarship Support</li>
	  		</ul>
		</div>

	</div>
	</div>

			<?php
		}
	}
?>
