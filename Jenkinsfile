pipeline {
    options {
      timeout(time: 1, unit: 'HOURS') 
    }
    agent {
        docker 'cuberite/docker-ci/minimal:latest'
    }
    stages {
        stage("Prepare") {
            steps {
                sh 'git submodule update --init'
            }
        }
        stage("Build") {
		    environment {
                CI_CUBERITE_BUILD_TYPE = 'Debug'
                CI_JOB_NUMBER = "{$env.BUILD_ID}"
                CC = "clang"
                CXX = "clang++"
            }
            steps {
                sh 'bash ./cibuild.sh'
            }
        }
		stage("Artifacts") {
			when {
				branch 'master'
			}
            steps {
                archiveArtifacts artifacts: 'gcc_Release/Server/.luacheckrc'
            }
        }
    }
    post {
        always {
            cleanWs()
        }
    }
}
