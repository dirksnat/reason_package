<?php
/**
 * Dorian Senior High Camp Module
 *
 * @author Steve Smith
 * @author Lucas Welper
 * @since 2011-01-26
 * @package Local Modules
 */

/**
 * needs default module
 */
reason_include_once( 'minisite_templates/modules/default.php' );
/**
 * needs Disco
 */
include_once(DISCO_INC . 'disco.php');

$GLOBALS[ '_module_class_names' ][ basename( __FILE__, '.php' ) ] = 'NorgeFormModule';

class NorgeFormModule extends DefaultMinisiteModule
{
	/**
	 * Before we clean the request vars, we need to init the controller so we know what we're initing
	 */
	function pre_request_cleanup_init()
	{
		include_once( DISCO_INC.'controller.php' );
		reason_include_once( 'minisite_templates/modules/norge_form/page1.php' );
		reason_include_once( 'minisite_templates/modules/norge_form/page2.php' );

		$this->controller = new FormController;
		$this->controller->set_session_class('Session_PHP');
		$this->controller->set_session_name('REASON_SESSION');
		$this->controller->set_data_context('norge_form');
		$this->controller->show_back_button = true;
		$this->controller->clear_form_data_on_finish = true;
		$this->controller->allow_arbitrary_start = true;
		//*
		$forms = array(
			'NorgeFormOne' => array(
				'next_steps' => array(
					'NorgeFormTwo' => array(
						'label' => 'Next'),
				),
				'step_decision' => array('type'=>'user'),
				'back_button_text' => 'Back',
			),
			'NorgeFormTwo' => array(
                            'final_step' => true,
                            'final_button_text' => 'Finish and Pay',
			),
		);
		$this->controller->add_forms( $forms );
		// */
		$this->controller->init();
	}

	/**
	 * Add possible forms variables that may come through to the list of vetted request vars
	 * @return void
	 */
	function get_cleanup_rules()
	{
		$rules = array();
		// debug var - resets form and destroys session
		$rules[ 'ds' ] = array( 'function' => 'turn_into_string' );
		// vars for confirmation page to let through
		$rules[ 'r' ] = array( 'function' => 'turn_into_string' );
		$rules[ 'h' ] = array( 'function' => 'turn_into_string' );
		// Allows form to be put into testing mode through a query string
		$rules[ 'tm' ] = array( 'function' => 'turn_into_int' );
		// add all cleanup rules from the form controller
		$rules = array_merge( $rules, $this->controller->get_cleanup_rules() );
		return $rules;
	}
        function init( $args = array() ) //{{{
	{
		parent::init( $args );

		if($head_items =& $this->get_head_items())
		{
                    $head_items->add_javascript('/reason/js/norge_form.js');
		}
	}
	/**
	 * Set up the request for the controller and run the sucker
	 * @return void
	 */
	function run()
	{
		if ( !empty( $this->request[ 'r' ] ) AND !empty( $this->request[ 'h' ] ) )
		{
			reason_include_once( 'minisite_templates/modules/norge_form/norge_confirmation.php' );
			$nc = new NorgeConfirmation;
			$nc->set_ref_number( $this->request[ 'r' ] );
			$nc->set_hash( $this->request[ 'h' ] );

			if ( $nc->validates() )
			{
				echo $nc->get_confirmation_text();
			}
			else
			{
				echo $nc->get_error_message();
			}
			// MUST reconnect to Reason database.
                        // NorgeConfirmation connects to naha_norge for info.
			connectDB( REASON_DB );
		}
		else
		{
			$this->controller->set_request( $this->request );
			$this->controller->run();
		}
	}
}
?>